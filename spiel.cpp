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

Spiel::Spiel() : aktuellerSpieler(0), lastWuerfel1(1), lastWuerfel2(1), eingabeHinweis(""), anweisungsText("") {
    verfuegbareFarben = {1, 2, 3, 4}; // 1=Rot, 2=Blau, 3=Grün, 4=Gelb
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
    use_default_colors(); // Ermöglicht Transparenz
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
//                printCentered(stdscr, 8, width, "Ungueltige Anzahl. Bitte erneut eingeben.");
//                refresh();
//                napms(1500); // Kurze Pause, um die Nachricht anzuzeigen
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
//                        printCentered(stdscr, 9, width, "Der Name darf nicht leer sein. Bitte erneut eingeben.");
//                        refresh();
//                        napms(1500); // Kurze Pause, um die Nachricht anzuzeigen
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

    // Initiale Anzeige
    aktualisiereBild();

    while (true) {
        // Aktueller Spieler würfelt und zieht
        wuerfelnUndZiehen();

        // Nächster Spieler
        aktuellerSpieler = (aktuellerSpieler + 1) % spieler.size();
    }

    endwin();
}

void Spiel::aktualisiereBild() {
    // Gesamten Bildschirm löschen
    clear();

    // Spielfeld zeichnen
    zeichneSpielfeld();

    // Anweisungsbereich zeichnen
    zeichneAnweisungsbereich();

    // Würfel zeichnen
    zeichneWuerfel();

    // Statuszeilen zeichnen
    zeichneStatuszeilen();

    // Eingabehinweise anzeigen
    zeichneEingabeHinweise();

    // Bildschirm aktualisieren
    refresh();
}

void Spiel::zeichneSpielfeld() {
    // Lösche nur den Bereich des Spielfelds (oberhalb des Interaktionsbereichs)
    for (int i = 0; i < LINES - 14; ++i) {
        move(i, 0);
        clrtoeol();
    }

    // Rahmen zeichnen
    box(stdscr, 0, 0);

    // Titel zentriert anzeigen
    int width = COLS;
    printCentered(stdscr, 0, width, "DKT");

    // Leerzeile statt Tabellenkopf
    mvprintw(1, 1, " ");
    mvprintw(1, width / 2, " ");

    // Erste Spalte (Felder 1-20)
    for (int i = 0; i < 20; i++) {
        Spielfeld& feld = spielfelder[i];
        int row = 2 + i;

        // Spieler auf dem Feld sammeln
        std::vector<Spieler*> spielerAufFeld;
        for (auto& sp : spieler) {
            if (sp.position == i) {
                spielerAufFeld.push_back(&sp);
            }
        }

        // Spielernamen formatieren
        if (!spielerAufFeld.empty()) {
            int nameLength = 12 / spielerAufFeld.size();
            if (nameLength < 3) nameLength = 3; // Mindestens 3 Zeichen pro Spieler

            int totalLength = nameLength * spielerAufFeld.size();
            if (totalLength > 12) totalLength = 12; // Maximal 12 Zeichen

            int startX = 1; // Startposition für Spielernamen
            for (size_t j = 0; j < spielerAufFeld.size(); j++) {
                Spieler* sp = spielerAufFeld[j];
                // Namen kürzen
                std::string name = sp->name.substr(0, nameLength);
                // Hintergrundfarbe setzen
                attron(COLOR_PAIR(sp->farbe));
                mvprintw(row, startX + j * nameLength, "%-*s", nameLength, name.c_str());
                attroff(COLOR_PAIR(sp->farbe));
            }
        } else {
            mvprintw(row, 1, "            "); // Leeres Feld für Spieler
        }

        // Feldinformationen anzeigen
        std::string fieldInfo;
        if (!feld.eigentuemer.empty()) {
            // Feld gehört jemandem, Mietpreis anzeigen
            int miete = feld.preis / 10; // Beispielhafter Mietpreis
            fieldInfo = "| " + std::to_string(feld.nummer) + ". | " + feld.name + " | Miete: " + std::to_string(miete) + " EUR";
            // Hintergrundfarbe des Eigentümers setzen
            int farbe = 0;
            for (const auto& sp : spieler) {
                if (sp.name == feld.eigentuemer) {
                    farbe = sp.farbe;
                    break;
                }
            }
            if (farbe != 0) {
                attron(COLOR_PAIR(farbe));
                mvprintw(row, 14, "%s", fieldInfo.c_str());
                attroff(COLOR_PAIR(farbe));
            } else {
                mvprintw(row, 14, "%s", fieldInfo.c_str());
            }
        } else {
            // Feld ist frei, Kaufpreis anzeigen
            fieldInfo = "| " + std::to_string(feld.nummer) + ". | " + feld.name + " | Kauf: " + std::to_string(feld.preis) + " EUR";
            mvprintw(row, 14, "%s", fieldInfo.c_str());
        }
    }

    // Zweite Spalte (Felder 21-40)
    for (int i = 20; i < 40; i++) {
        Spielfeld& feld = spielfelder[i];
        int row = 2 + (i - 20);

        // Spieler auf dem Feld sammeln
        std::vector<Spieler*> spielerAufFeld;
        for (auto& sp : spieler) {
            if (sp.position == i) {
                spielerAufFeld.push_back(&sp);
            }
        }

        // Spielernamen formatieren
        if (!spielerAufFeld.empty()) {
            int nameLength = 12 / spielerAufFeld.size();
            if (nameLength < 3) nameLength = 3; // Mindestens 3 Zeichen pro Spieler

            int totalLength = nameLength * spielerAufFeld.size();
            if (totalLength > 12) totalLength = 12; // Maximal 12 Zeichen

            int startX = width / 2 + 1; // Startposition für Spielernamen
            for (size_t j = 0; j < spielerAufFeld.size(); j++) {
                Spieler* sp = spielerAufFeld[j];
                // Namen kürzen
                std::string name = sp->name.substr(0, nameLength);
                // Hintergrundfarbe setzen
                attron(COLOR_PAIR(sp->farbe));
                mvprintw(row, startX + j * nameLength, "%-*s", nameLength, name.c_str());
                attroff(COLOR_PAIR(sp->farbe));
            }
        } else {
            mvprintw(row, width / 2 + 1, "            "); // Leeres Feld für Spieler
        }

        // Feldinformationen anzeigen
        std::string fieldInfo;
        if (!feld.eigentuemer.empty()) {
            // Feld gehört jemandem, Mietpreis anzeigen
            int miete = feld.preis / 10; // Beispielhafter Mietpreis
            fieldInfo = "| " + std::to_string(feld.nummer) + ". | " + feld.name + " | Miete: " + std::to_string(miete) + " EUR";
            // Hintergrundfarbe des Eigentümers setzen
            int farbe = 0;
            for (const auto& sp : spieler) {
                if (sp.name == feld.eigentuemer) {
                    farbe = sp.farbe;
                    break;
                }
            }
            if (farbe != 0) {
                attron(COLOR_PAIR(farbe));
                mvprintw(row, width / 2 + 14, "%s", fieldInfo.c_str());
                attroff(COLOR_PAIR(farbe));
            } else {
                mvprintw(row, width / 2 + 14, "%s", fieldInfo.c_str());
            }
        } else {
            // Feld ist frei, Kaufpreis anzeigen
            fieldInfo = "| " + std::to_string(feld.nummer) + ". | " + feld.name + " | Kauf: " + std::to_string(feld.preis) + " EUR";
            mvprintw(row, width / 2 + 14, "%s", fieldInfo.c_str());
        }
    }

    // Rahmen um den Interaktionsbereich zeichnen
    int interactionStartRow = LINES - 14;
    for (int i = interactionStartRow; i < LINES - 1; ++i) {
        move(i, 0);
        clrtoeol();
    }
    // Rahmen zeichnen
    mvhline(interactionStartRow, 0, ACS_HLINE, COLS);
    mvhline(LINES - 1, 0, ACS_HLINE, COLS);
    mvvline(interactionStartRow, 0, ACS_VLINE, LINES - interactionStartRow);
    mvvline(interactionStartRow, COLS - 1, ACS_VLINE, LINES - interactionStartRow);
    mvaddch(interactionStartRow, 0, ACS_ULCORNER);
    mvaddch(interactionStartRow, COLS - 1, ACS_URCORNER);
    mvaddch(LINES - 1, 0, ACS_LLCORNER);
    mvaddch(LINES - 1, COLS - 1, ACS_LRCORNER);

    // Spielerzustände zentriert und farblich hinterlegt anzeigen
    int statusRow = LINES - 14;
    mvhline(statusRow, 0, ACS_HLINE, COLS); // Trennlinie

    int xPos = (COLS - (spieler.size() * 20)) / 2; // Berechne Startposition
    for (const auto& sp : spieler) {
        std::string info = sp.name + ": " + std::to_string(sp.geld) + " EUR  ";
        attron(COLOR_PAIR(sp.farbe));
        mvprintw(statusRow, xPos, "%s", info.c_str());
        attroff(COLOR_PAIR(sp.farbe));
        xPos += info.length();
    }

    // Eingabehinweise zentriert in der untersten Zeile anzeigen
    std::string hint = eingabeHinweis; // Variable eingabeHinweis speichern
    printCentered(stdscr, LINES - 1, COLS, hint.c_str());

    refresh();
}

void Spiel::zeichneAnweisungsbereich() {
    int interactionStartRow = LINES - 12; // Startzeile für Anweisungsbereich
    int width = COLS;

    // Anweisungsbereich löschen
    int instructionAreaWidth = width * 2 / 3;
    for (int i = interactionStartRow; i < LINES - 2; ++i) {
        move(i, 1);
        clrtoeol();
    }

    // Anweisungstext anzeigen
    printCentered(stdscr, interactionStartRow, instructionAreaWidth, anweisungsText.c_str());
}

void Spiel::zeichneWuerfel() {
    int startRow = LINES - 14;
    int width = COLS;
    int diceAreaStartCol = width * 2 / 3 + 1;

    // Würfelbereich löschen
    for (int i = startRow; i < LINES - 2; ++i) {
        move(i, diceAreaStartCol);
        clrtoeol();
    }

    int dice1_col = diceAreaStartCol + 5;
    int dice2_col = diceAreaStartCol + 15;

    const char* diceFaces[6][5] = {
        {"+-----+", "|     |", "|  O  |", "|     |", "+-----+"},
        {"+-----+", "| O   |", "|     |", "|   O |", "+-----+"},
        {"+-----+", "| O   |", "|  O  |", "|   O |", "+-----+"},
        {"+-----+", "| O O |", "|     |", "| O O |", "+-----+"},
        {"+-----+", "| O O |", "|  O  |", "| O O |", "+-----+"},
        {"+-----+", "| O O |", "| O O |", "| O O |", "+-----+"}
    };

    int w1 = lastWuerfel1 - 1;
    int w2 = lastWuerfel2 - 1;

    if (w1 < 0 || w1 >= 6) w1 = 0;
    if (w2 < 0 || w2 >= 6) w2 = 0;

    for (int j = 0; j < 5; ++j) {
        mvprintw(startRow + j + 2, dice1_col, "%s", diceFaces[w1][j]);
        mvprintw(startRow + j + 2, dice2_col, "%s", diceFaces[w2][j]);
    }
}

void Spiel::zeichneStatuszeilen() {
    int statusRow = LINES - 14;
    int width = COLS;

    // Horizontale Linie zeichnen
    mvhline(statusRow, 0, ACS_HLINE, width);

    // Spielerzustände anzeigen
    int xPos = (width - (spieler.size() * 20)) / 2; // Startposition berechnen
    for (const auto& sp : spieler) {
        std::string info = sp.name + ": " + std::to_string(sp.geld) + " EUR  ";
        attron(COLOR_PAIR(sp.farbe));
        mvprintw(statusRow + 1, xPos, "%s", info.c_str());
        attroff(COLOR_PAIR(sp.farbe));
        xPos += info.length();
    }
}

void Spiel::zeichneEingabeHinweise() {
    // Eingabehinweise zentriert unten anzeigen
    printCentered(stdscr, LINES - 1, COLS, eingabeHinweis.c_str());
}

void Spiel::wuerfelnUndZiehen() {
    Spieler& aktSpieler = spieler[aktuellerSpieler];

    bool nochmalWuerfeln = true;

    while (nochmalWuerfeln) {
        // Anweisungstext aktualisieren
        anweisungsText = aktSpieler.name + " ist dran.";
        // Eingabehinweis aktualisieren
        eingabeHinweis = " [Enter/Space] zum Würfeln ";

        // Gesamtes Bild aktualisieren
        aktualisiereBild();

        // Warten auf Eingabe
        int ch;
        do {
            ch = getch();
        } while (ch != '\n' && ch != ' ');

        // Würfelanimation
        int wuerfel1, wuerfel2;
        animateDice(wuerfel1, wuerfel2);

        // Letzte Würfelwerte speichern
        lastWuerfel1 = wuerfel1;
        lastWuerfel2 = wuerfel2;

        int schritte = wuerfel1 + wuerfel2;

        // Bewegung der Spielfigur mit Animation
        animateMovement(aktSpieler, schritte);

        // Gesamtes Bild aktualisieren
        aktualisiereBild();

        // Feldaktion durchführen
        feldAktion(aktSpieler);

        // Gesamtes Bild erneut aktualisieren
        aktualisiereBild();

        // Prüfen auf Pasch
        if (wuerfel1 == wuerfel2) {
            anweisungsText = "Du darfst nochmal würfeln.";
            eingabeHinweis = "";
            aktualisiereBild();
            napms(2000);
            nochmalWuerfeln = true;
        } else {
            nochmalWuerfeln = false;
        }
    }
}

void Spiel::animateDice(int& w1, int& w2) {
    int delays[] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 150, 200, 250, 300};

    for (int i = 0; i < sizeof(delays) / sizeof(int); ++i) {
        w1 = rand() % 6;
        w2 = rand() % 6;

        // Temporäre Würfelwerte speichern
        lastWuerfel1 = w1 + 1;
        lastWuerfel2 = w2 + 1;

        // Bild aktualisieren, um die Würfel anzuzeigen
        aktualisiereBild();

        napms(delays[i]);
    }

    // Endgültige Würfelwerte
    w1 += 1;
    w2 += 1;
}

void Spiel::animateMovement(Spieler& sp, int schritte) {
    for (int i = 0; i < schritte; ++i) {
        sp.position = (sp.position + 1) % 40;
        // Gesamtes Bild aktualisieren
        aktualisiereBild();
        // Kurze Pause für die Animation
        napms(250);
    }
}

void Spiel::feldAktion(Spieler& sp) {
    Spielfeld& feld = spielfelder[sp.position];

    if (feld.eigentuemer.empty()) {
        // Kaufoption anbieten
        anweisungsText = sp.name + ", willst du \"" + feld.name + "\" für " + std::to_string(feld.preis) + " EUR kaufen?";
        eingabeHinweis = " [J/N] ";

        // Gesamtes Bild aktualisieren
        aktualisiereBild();

        int ch;
        do {
            ch = toupper(getch());
        } while (ch != 'J' && ch != 'N');

        if (ch == 'J') {
            if (sp.geld >= feld.preis) {
                sp.geld -= feld.preis;
                feld.eigentuemer = sp.name;
                anweisungsText = "Gekauft: " + feld.name;
                eingabeHinweis = "";
                aktualisiereBild();
                napms(2000);
            } else {
                anweisungsText = "Nicht genug Geld.";
                eingabeHinweis = "";
                aktualisiereBild();
                napms(2000);
            }
        } else {
            // Spieler lehnt ab
            anweisungsText = "";
            eingabeHinweis = "";
        }
    } else if (feld.eigentuemer != sp.name) {
        // Miete zahlen
        int miete = feld.preis / 10; // Beispielhafte Miete
        sp.geld -= miete;

        // Eigentümer finden und Miete gutschreiben
        for (auto& eigSp : spieler) {
            if (eigSp.name == feld.eigentuemer) {
                eigSp.geld += miete;
                break;
            }
        }

        anweisungsText = "Du zahlst " + std::to_string(miete) + " EUR Miete an " + feld.eigentuemer;
        eingabeHinweis = "";
        aktualisiereBild();
        napms(2000);
    } else {
        // Nichts passiert
        anweisungsText = "";
        eingabeHinweis = "";
    }
}
