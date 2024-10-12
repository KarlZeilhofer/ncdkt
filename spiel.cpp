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
        int preis = ((i + 1) * 10) % 200 + 50; // Beispielpreise zwischen 50 und 250
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

    int anzahlSpieler = 0;
    bool spielerAnzahlEingegeben = false;
    std::vector<std::string> spielerNamen;
    std::vector<int> spielerFarben;

    while (true) {
        // Fenstergröße ermitteln
        int height, width;
        getmaxyx(stdscr, height, width);

        // Bildschirm löschen und neu zeichnen
        clear();
        box(stdscr, 0, 0);

        // Titel zentriert
        printCentered(stdscr, 2, width, "Willkommen bei DKT!");

        if (!spielerAnzahlEingegeben) {
            printCentered(stdscr, 4, width, "Bitte geben Sie die Anzahl der Spieler ein (2-4): ");
            move(6, width / 2 - 2);
            echo();
            char input[3];
            getnstr(input, 2);
            noecho();

            anzahlSpieler = atoi(input);

            // Überprüfen der Spieleranzahl
            if (anzahlSpieler >= 2 && anzahlSpieler <= 4) {
                spielerAnzahlEingegeben = true;
            } else {
                printCentered(stdscr, 8, width, "Ungueltige Anzahl. Bitte erneut eingeben.");
                refresh();
                napms(1500); // Kurze Pause, um die Nachricht anzuzeigen
            }
        } else {
            for (int i = 0; i < anzahlSpieler; i++) {
                bool eingabeErfolgreich = false;
                while (!eingabeErfolgreich) {
                    // Fenstergröße ermitteln
                    getmaxyx(stdscr, height, width);

                    // Bildschirm löschen und neu zeichnen
                    clear();
                    box(stdscr, 0, 0);

                    // Titel zentriert
                    printCentered(stdscr, 2, width, "Willkommen bei DKT!");
                    std::string spielerInfo = "Spieler " + std::to_string(i + 1) + " von " + std::to_string(anzahlSpieler);
                    printCentered(stdscr, 4, width, spielerInfo.c_str());

                    printCentered(stdscr, 6, width, "Name des Spielers:");
                    move(7, width / 2 - 10);
                    echo();
                    char name[50];
                    getnstr(name, sizeof(name) - 1);
                    noecho();

                    // Überprüfen, ob der Name leer ist
                    if (strlen(name) == 0) {
                        printCentered(stdscr, 9, width, "Der Name darf nicht leer sein. Bitte erneut eingeben.");
                        refresh();
                        napms(1500); // Kurze Pause, um die Nachricht anzuzeigen
                        continue; // Zurück zum Anfang der Schleife
                    }

                    // Farbauswahl
                    int farbe = zeigeFarbauswahl(stdscr, 11);

                    // Spielername und Farbe speichern
                    spielerNamen.push_back(std::string(name));
                    spielerFarben.push_back(farbe);

                    // Gewählte Farbe aus verfügbaren Farben entfernen
                    verfuegbareFarben.erase(std::remove(verfuegbareFarben.begin(), verfuegbareFarben.end(), farbe), verfuegbareFarben.end());

                    eingabeErfolgreich = true;
                }
            }

            // Spieler initialisieren
            for (size_t i = 0; i < spielerNamen.size(); i++) {
                spieler.push_back(Spieler(spielerNamen[i], spielerFarben[i], 0, 1500));
            }

            break; // Willkommensbildschirm beenden
        }

        // Auf Tastendruck prüfen (inklusive KEY_RESIZE)
        nodelay(stdscr, TRUE); // Nicht blockierend
        int ch = getch();
        if (ch == KEY_RESIZE) {
            // Fenstergröße wurde geändert, Schleife erneut durchlaufen, um Bildschirm neu zu zeichnen
            continue;
        }
        nodelay(stdscr, FALSE); // Zurück zu blockierend

        refresh();
    }

    // End NCurses-Modus
    endwin();
}

int Spiel::zeigeFarbauswahl(WINDOW* win, int starty) {
    int highlight = 0; // Index der hervorgehobenen Farbe
    int choice = -1;
    int c;

    keypad(win, TRUE); // Pfeiltasten im Fenster aktivieren

    while (choice == -1) {
        // Fenstergröße ermitteln
        int width = getmaxx(win);

        werase(win);
        box(win, 0, 0);

        printCentered(win, starty - 2, width, "Waehle eine Farbe mit den Pfeiltasten und druecke Enter:");

        for (size_t i = 0; i < verfuegbareFarben.size(); i++) {
            int x = width / 2 - 2; // Zentriert anzeigen
            if ((int)i == highlight) {
                // Markiere die aktuelle Auswahl mit einem Pfeil
                mvwprintw(win, starty + i, x - 4, "-->");
            } else {
                mvwprintw(win, starty + i, x - 4, "   ");
            }

            // Farben anzeigen
            switch (verfuegbareFarben[i]) {
                case 1:
                    wattron(win, COLOR_PAIR(1));
                    mvwprintw(win, starty + i, x, "Rot");
                    wattroff(win, COLOR_PAIR(1));
                    break;
                case 2:
                    wattron(win, COLOR_PAIR(2));
                    mvwprintw(win, starty + i, x, "Blau");
                    wattroff(win, COLOR_PAIR(2));
                    break;
                case 3:
                    wattron(win, COLOR_PAIR(3));
                    mvwprintw(win, starty + i, x, "Gruen");
                    wattroff(win, COLOR_PAIR(3));
                    break;
                case 4:
                    wattron(win, COLOR_PAIR(4));
                    mvwprintw(win, starty + i, x, "Gelb");
                    wattroff(win, COLOR_PAIR(4));
                    break;
            }
        }

        wrefresh(win);

        nodelay(win, TRUE); // Nicht blockierend
        c = wgetch(win);
        if (c == ERR) {
            // Keine Eingabe, aber wir können prüfen, ob das Fenster resized wurde
            int newWidth = getmaxx(win);
            if (newWidth != width) {
                continue; // Fenstergröße hat sich geändert, neu zeichnen
            }
        } else if (c == KEY_RESIZE) {
            // Fenstergröße geändert, neu zeichnen
            continue;
        } else {
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
        nodelay(win, FALSE); // Zurück zu blockierend
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

    // Rahmen zeichnen
    box(stdscr, 0, 0);

    // Titel zentriert anzeigen
    int width = COLS;
    printCentered(stdscr, 0, width, "DKT");

    mvprintw(1, 1, "Nr.  | Name                       | Preis ");

    for (size_t i = 0; i < spielfelder.size(); i++) {
        Spielfeld& feld = spielfelder[i];
        char line[100];
        snprintf(line, sizeof(line), "%2d. | %-25s | %6d EUR", feld.nummer, feld.name.c_str(), feld.preis);

        // Wenn das Feld einem Spieler gehört, Hintergrundfarbe setzen
        if (!feld.eigentuemer.empty()) {
            for (const auto& sp : spieler) {
                if (sp.name == feld.eigentuemer) {
                    attron(COLOR_PAIR(sp.farbe));
                    mvprintw(2 + i, 1, "%s", line);
                    attroff(COLOR_PAIR(sp.farbe));
                    break;
                }
            }
        } else {
            mvprintw(2 + i, 1, "%s", line);
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

    mvprintw(LINES - 5, 1, "Spieler %s ist an der Reihe. Druecken Sie eine Taste zum Wuerfeln.", aktSpieler.name.c_str());
    getch();

    bool nochmalWuerfeln = true;

    while (nochmalWuerfeln) {
        int wuerfel1 = (rand() % 6) + 1;
        int wuerfel2 = (rand() % 6) + 1;
        int schritte = wuerfel1 + wuerfel2;

        mvprintw(LINES - 4, 1, "Gewuerfelt: %d + %d = %d", wuerfel1, wuerfel2, schritte);

        // Spieler bewegen
        aktSpieler.bewegeVorwaerts(schritte);

        // Spielfeld neu zeichnen
        zeichneSpielfeld();

        // Prüfen auf Pasch
        if (wuerfel1 == wuerfel2) {
            mvprintw(LINES - 3, 1, "Doppelwurf! Sie duerfen erneut wuerfeln.");
            getch();
            nochmalWuerfeln = true;
        } else {
            nochmalWuerfeln = false;
        }
    }
}
