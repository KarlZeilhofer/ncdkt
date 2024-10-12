#include "spiel.h"
#include <cstring>   // Für strlen
#include <string>
#include <algorithm> // Für std::remove
#include <cstdlib>   // Für rand()
#include <ctime>     // Für time()

// Hilfsfunktion, um Text zentriert auszugeben
void printCentered(WINDOW* win, int starty, int width, const char* text) {
    int length = strlen(text);
    int x = (width - length) / 2;
    mvwprintw(win, starty, x, "%s", text);
}

Spiel::Spiel() : aktuellerSpieler(0) {
    verfuegbareFarben = {1, 2, 3, 4}; // 1=Rot, 2=Blau, 3=Gruen, 4=Gelb
    srand(time(0)); // Zufallszahlengenerator initialisieren

    // Spielfelder initialisieren
    for (int i = 0; i < 40; i++) {
        std::string name = "Feld " + std::to_string(i + 1);
        int preis = ((i + 1) * 10) % 200 + 50; // Beispielpreise zwischen 50€ und 250€
        spielfelder.push_back(Spielfeld(i + 1, name, preis));
    }
}

void Spiel::setzeFarben() {
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_RED);    // Rot
    init_pair(2, COLOR_WHITE, COLOR_BLUE);   // Blau
    init_pair(3, COLOR_WHITE, COLOR_GREEN);  // Grün
    init_pair(4, COLOR_BLACK, COLOR_YELLOW); // Gelb
}

void Spiel::willkommenBildschirm() {
    initscr();
    curs_set(1);
    noecho();
    start_color();
    setzeFarben();
    keypad(stdscr, TRUE); // Pfeiltasten aktivieren

    int height, width;
    getmaxyx(stdscr, height, width);

    // Neues Fenster erstellen
    WINDOW* win = newwin(height - 2, width - 2, 1, 1);
    box(win, 0, 0);

    // Anzahl der Spieler abfragen
    mvwprintw(win, 2, 2, "Willkommen bei DKT!");
    mvwprintw(win, 4, 2, "Bitte geben Sie die Anzahl der Spieler ein (2-4): ");
    wrefresh(win);

    int anzahlSpieler;
    echo();
    mvwscanw(win, 5, 2, "%d", &anzahlSpieler);
    noecho();

    // Überprüfen der Spieleranzahl
    while (anzahlSpieler < 2 || anzahlSpieler > 4) {
        mvwprintw(win, 6, 2, "Ungueltige Anzahl. Bitte waehle zwischen 2 und 4 Spielern.");
        wrefresh(win);
        echo();
        mvwscanw(win, 7, 2, "%d", &anzahlSpieler);
        noecho();
    }

    // Spielernamen und Farben eingeben
    for (int i = 0; i < anzahlSpieler; i++) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, 2, 2, "Spieler %d von %d", i + 1, anzahlSpieler);
        mvwprintw(win, 4, 2, "Name des Spielers:");
        wrefresh(win);

        char name[50];
        echo(); // Echo aktivieren für Namenseingabe
        mvwgetnstr(win, 5, 2, name, sizeof(name) - 1); // Spielernamen eingeben
        noecho(); // Echo wieder deaktivieren

        // Farbauswahl mit Pfeiltasten
        int farbe = zeigeFarbauswahl(win, 7);

        // Spieler initialisieren
        spieler.push_back(Spieler(name, farbe, 0, 1500)); // Startposition 0, Startgeld 1500€

        // Gewählte Farbe aus verfügbaren Farben entfernen
        verfuegbareFarben.erase(std::remove(verfuegbareFarben.begin(), verfuegbareFarben.end(), farbe), verfuegbareFarben.end());
    }

    // Fenster schließen
    werase(win);
    wrefresh(win);
    delwin(win);
    endwin();
}

int Spiel::zeigeFarbauswahl(WINDOW* win, int starty) {
    int highlight = 0; // Index der hervorgehobenen Farbe
    int choice = -1;
    int c;

    keypad(win, TRUE); // Pfeiltasten im Fenster aktivieren

    while (choice == -1) {
        werase(win);
        box(win, 0, 0);
        mvwprintw(win, starty - 2, 2, "Waehle eine Farbe mit den Pfeiltasten und druecke Enter:");
        for (size_t i = 0; i < verfuegbareFarben.size(); i++) {
            if ((int)i == highlight) {
                // Markiere die aktuelle Auswahl mit einem Pfeil
                mvwprintw(win, starty + i, 2, "-->");
            } else {
                mvwprintw(win, starty + i, 2, "   ");
            }

            // Farben anzeigen
            switch (verfuegbareFarben[i]) {
                case 1:
                    wattron(win, COLOR_PAIR(1));
                    mvwprintw(win, starty + i, 6, "Rot");
                    wattroff(win, COLOR_PAIR(1));
                    break;
                case 2:
                    wattron(win, COLOR_PAIR(2));
                    mvwprintw(win, starty + i, 6, "Blau");
                    wattroff(win, COLOR_PAIR(2));
                    break;
                case 3:
                    wattron(win, COLOR_PAIR(3));
                    mvwprintw(win, starty + i, 6, "Gruen");
                    wattroff(win, COLOR_PAIR(3));
                    break;
                case 4:
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, starty + i, 6, "Gelb");
                    wattroff(win, COLOR_PAIR(4));
                    break;
            }
        }

        wrefresh(win);

        c = wgetch(win);
        switch (c) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = verfuegbareFarben.size() - 1;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight >= (int)verfuegbareFarben.size()) highlight = 0;
                break;
            case 10: // Enter-Taste
                choice = verfuegbareFarben[highlight];
                break;
            default:
                break;
        }
    }

    return choice;
}

void Spiel::spielLoop() {
    initscr();
    curs_set(0);
    noecho();
    start_color();
    setzeFarben();
    keypad(stdscr, TRUE);

    while (true) {
        // Spielfeld zeichnen
        zeichneSpielfeld();

        // Aktueller Spieler würfelt und zieht
        wuerfelnUndZiehen();

        // Warten auf Tastendruck
        mvprintw(LINES - 1, 0, "Druecken Sie eine Taste, um fortzufahren...");
        getch();

        // Nächster Spieler
        aktuellerSpieler = (aktuellerSpieler + 1) % spieler.size();
    }

    endwin();
}

void Spiel::zeichneSpielfeld() {
    clear();

    mvprintw(0, 0, "Spielfeld:");
    mvprintw(1, 0, "Nr.  | Name                       | Preis  ");

    for (size_t i = 0; i < spielfelder.size(); i++) {
        Spielfeld& feld = spielfelder[i];
        char line[100];
        snprintf(line, sizeof(line), "%2d. | %-25s | %6d€", feld.nummer, feld.name.c_str(), feld.preis);

        // Wenn das Feld einem Spieler gehört, Hintergrundfarbe setzen
        if (!feld.eigentuemer.empty()) {
            for (const auto& sp : spieler) {
                if (sp.name == feld.eigentuemer) {
                    attron(COLOR_PAIR(sp.farbe));
                    mvprintw(2 + i, 0, "%s", line);
                    attroff(COLOR_PAIR(sp.farbe));
                    break;
                }
            }
        } else {
            mvprintw(2 + i, 0, "%s", line);
        }

        // Spieler auf dem Feld anzeigen
        for (const auto& sp : spieler) {
            if (sp.position == (int)i) {
                attron(COLOR_PAIR(sp.farbe));
                mvprintw(2 + i, COLS - 15, "[%s]", sp.name.c_str());
                attroff(COLOR_PAIR(sp.farbe));
            }
        }
    }

    refresh();
}

void Spiel::wuerfelnUndZiehen() {
    Spieler& aktSpieler = spieler[aktuellerSpieler];

    mvprintw(LINES - 3, 0, "Spieler %s ist an der Reihe. Druecken Sie eine Taste zum Wuerfeln.", aktSpieler.name.c_str());
    getch();

    bool nochmalWuerfeln = true;

    while (nochmalWuerfeln) {
        int wuerfel1 = (rand() % 6) + 1;
        int wuerfel2 = (rand() % 6) + 1;
        int schritte = wuerfel1 + wuerfel2;

        mvprintw(LINES - 2, 0, "Gewuerfelt: %d + %d = %d", wuerfel1, wuerfel2, schritte);

        // Spieler bewegen
        aktSpieler.bewegeVorwaerts(schritte);

        // Spielfeld neu zeichnen
        zeichneSpielfeld();

        // Prüfen auf Pasch
        if (wuerfel1 == wuerfel2) {
            mvprintw(LINES - 1, 0, "Doppelwurf! Sie duerfen erneut wuerfeln.");
            getch();
            nochmalWuerfeln = true;
        } else {
            nochmalWuerfeln = false;
        }
    }
}
