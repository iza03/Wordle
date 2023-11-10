#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
void creare_lista(char v[28][6])
{ // pun in vector cuvintele date
    strcpy(v[0], "arici");
    strcpy(v[7], "hamac");
    strcpy(v[14], "oaste");
    strcpy(v[21], "texte");
    strcpy(v[1], "atent");
    strcpy(v[8], "harta");
    strcpy(v[15], "perus");
    strcpy(v[22], "tipar");
    strcpy(v[2], "baiat");
    strcpy(v[9], "jalon");
    strcpy(v[16], "rigle");
    strcpy(v[23], "titan");
    strcpy(v[3], "ceata");
    strcpy(v[10], "jucam");
    strcpy(v[17], "roman");
    strcpy(v[24], "zebra");
    strcpy(v[4], "debut");
    strcpy(v[11], "lacat");
    strcpy(v[18], "sanie");
    strcpy(v[25], "vapor");
    strcpy(v[5], "peste");
    strcpy(v[12], "magie");
    strcpy(v[19], "scris");
    strcpy(v[26], "vatra");
    strcpy(v[6], "fixat");
    strcpy(v[13], "nufar");
    strcpy(v[20], "sonda");
}
void patrat(int stx, int sty)
{                                               // desenez o casuta in care va fi scrisa o litera, cu coltul din stanga sus de coordonate (stx,sty)
    mvhline(stx, sty, ACS_ULCORNER, 1);         // desenez coltul stanga sus
    mvhline(stx, sty + 1, ACS_HLINE, 6);        // desenez linia de sus
    mvhline(stx + 3, sty + 1, ACS_HLINE, 6);    // linia de jos
    mvhline(stx, sty + 6, ACS_URCORNER, 1);     // coltul din dreapta sus
    mvvline(stx + 1, sty, ACS_VLINE, 2);        // linia din stanga
    mvvline(stx + 3, sty, ACS_LLCORNER, 1);     // coltul din stanga jos
    mvvline(stx + 1, sty + 6, ACS_VLINE, 2);    // linia din dreapta
    mvvline(stx + 3, sty + 6, ACS_LRCORNER, 1); // coltul din dreapta jos
}
void draw(int centru)
{ // desenez ecranul jocului, apeland functia patrat pentru fiecare casuta
    int col = 0, lin = 0;
    while (col <= 20) // liniile verticale au 4 randuri, trebuie desenate de 6 ori
    {
        while (lin <= 32) // liniile orizontale au 8 spatii si se deseneaza de 5 ori
        {
            /*desenz un patrat cu coltul din stanga sus cu 4 mai jos decat ulrima coloana(initial 0)
            si mai la dreapta de centru cu lin spatii*/
            patrat(col + 4, lin + centru);
            lin += 8; // lin creste cu lungimea laturii de sus a patratului desenat
        }
        col += 4; // col creste cu lungimea laturii din stanga a patratului
        lin = 0;  // linia incepe de la 0, patratele fiind desenate de la stanga la dreapta
    }
}
void modif(int v[7], char s[7], char cuv[7])
{ /* functia modifica valorile vectorului v ce contine codificarea pentru colorarea literelor
 0->litera nu este in cuvant, 1->litera apare, dar pe alta pozitie, 2->litera este pe pozitia corecta
 la valorile de 0 si 2 stim sigur ca s-au salvat corect, la cele de 1 trebuie sa apara de atatea ori de cate
 se afla litera in cuvant, in afara celor de pe pozitia corecta*/
    int i, j, k, cate;
    for (i = 0; i <= 4; i++)
        if (v[i] == 1)
        { // cand gasesc o valoare de 1 aflu de cate ori se afla litera corespunzatoare in cuvant
            cate = 0;
            for (j = 0; j <= 4; j++)
                if (s[i] == cuv[j])
                    cate++;
            for (k = 0; k <= 4; k++) // vad de cate ori a fost introdusa litera si scad cate daca este pe pozitia corect
                if (s[k] == s[i] && v[k] == 2)
                    cate--;
            for (k = 0; k <= 4; k++)
                if (s[k] == s[i] && v[k] == 1)
                {             // de fiecare data cand a fost introdusa litera verific daca mai e nevoie de ea pt a scrie cuvantul
                    if (cate) // daca se mai afla in cuvant scad numarul
                        cate--;
                    else
                        v[k] = 0; // daca au fost codificate toate aparitiile necesare, celelalte devin 0
                }
        }
}
void color(int x, int y, int nr, char c)
{ /* functia coloreaza un patrat cu coltul din stanga sus de coordonate (x,y) cu culoarea de la perechea nr
 si in centru pune caracterul c*/
    int i;
    attron(COLOR_PAIR(nr));      // incep utilizarea culorii
    for (i = y; i <= y + 4; i++) // parcurg prima linie si o colorez
        if (i == y + 2)
            mvaddch(x, i, c); // pe pozitia care trebuie afisez caracterul
        else
            mvaddch(x, i, ' '); // in rest pun spatiu
    for (i = y; i <= y + 4; i++)
        mvaddch(x + 1, i, ' '); // parcurg si a doua linie si pun doar spatiu
    attroff(COLOR_PAIR(nr));    // opresc culoarea
}
void color_row(int x1, int y1, int corect[7], char cuv[7])
{ /* colorez primele 5 litere de la incercarea curenta, prima casuta are coordonatele coltului din stanga
 sus (x1,x2), vectorul cu valorile codificate este corect si cuvantul introdus de la tastatura este cuv */
    int i;
    for (i = 0; i <= 4; i++)
    {                  // colorez cate un patrat, daca are codificarea 0 ramane cu fundal negru si scris alb
        if (corect[i]) // daca trebuie colorat se face utilizeaza culoarea cu numarul codificarii(1/2)
            color(x1, y1, corect[i], cuv[i] - 32);
        y1 += 8; // se muta la casuta urmatoare
    }
}
int verif(int v[7])
{ // functia verifica daca s-a ghicit cuvantul, adica in vector se afla doar valoarea 2
    // se returneaza 1 daca este corect, 0 altfel
    int i, ok = 1;
    for (i = 0; i <= 4; i++)
    {
        if (v[i] < 2)
            ok = 0;
        v[i] = 0; // vectorul primeste 0 peste tot pentru urmatoarul cuvant introdus
    }
    return ok;
}
int incercare(WINDOW *wnd, int poz, char cuv[7], int corect[7], int pas)
{ // functia afiseaza literele si apeleaza functia pentru colorarea casutelor
    int i = 0, x, y, j, z;
    char c, primit[7];
    while (i <= 5)
    {
        c = getch();      // se citeste un caracter
        getyx(wnd, x, y); // x si y primesc pozitia cursorului
        if (c >= 'a' && c <= 'z' && i < 5)
        {                                            // daca s-a introdus o litera mica se afiseaza
            getyx(wnd, x, z);                        // aflam pozitia curenta
            move(3, poz);                            // mergem la pozitia la care a fost scris mesajul
            printw("                             "); // in caz ca a fost afisat mesajul pentru un cuvant mai scurt, se sterge
            move(x, z);                              // ne intoarcem la pozitia curenta
            i++;                                     // creste numarul de litere introduse
            if (strchr(cuv, c) && cuv[i - 1] == c)
                corect[i - 1] = 2; // daca este in cuvantul cautat si pe pozitia corecta, se salveaza 2
            else if (strchr(cuv, c))
                corect[i - 1] = 1; // daca se afla in cuvant dar pe alta pozitia, se salveaza 1
            primit[i - 1] = c;     // se salveaza litera introdusa
            c -= 32;               // se face litera mare
            addch(c);              // se afiseaza
            move(x, y + 8);        // se muta in casuta din dreapta
        }
        else if ((c == 8 || c == 127) && i > 0)
        {                                            // daca nu este primul caracter si este backspace
            getyx(wnd, x, z);                        // aflam pozitia curenta
            move(3, poz);                            // mergem la pozitia la care a fost scris mesajul
            printw("                             "); // in caz ca a fost afisat mesajul pentru un cuvant mai scurt, se sterge
            move(x, z);                              // ne intoarcem la pozitia curenta
            move(x, y - 8);                          // se muta in casuta din stanga
            addch(' ');                              // se sterge litera de acolo
            move(x, y - 8);                          // se muta din nou pe pozitia corecta
            i--;                                     // scade numarul de litere introduse
            corect[i] = 0;                           // valoarea corespunzatoare literei sterse se face 0
        }
        else if (c == '\n' && i != 5)
        {                                            // daca s-a apasat enter si nu s-au introdus deja 5 litere
            getyx(wnd, x, z);                        // aflam pozitia curenta
            move(3, poz);                            // ne mutam in locul pentru afisat mesaje
            printw("INTRODU UN CUVANT DE 5 LITERE"); // afisam mesajul, va fi sters dupa introducerea urmatoarei litere
            move(x, z);                              // ne intoarcem la pozitia initiala
        }
        else if (c == '\n' && i == 5)
            i++;          // daca s-a apasat enter dupa 5 litere, se termina citirea
        else if (c == 58) // daca s-a apasat ':' apare meniul de control
        {
            attron(COLOR_PAIR(3)); // se activeaza culoarea cu numarul 3(rosu)
            move(2, poz - 3);      // mergem la pozitia pentru prima optiune
            printw("Pentru a renunta la joc apasa tasta ENTER");
            move(3, poz - 3); // pentru a doua
            printw("Pentru a incepe un joc nou apasa tasta + ");
            attroff(COLOR_PAIR(3)); // se opreste culoarea
            c = getch();            // se primeste un caracter, enter s-au + si se returneaza numarul corespunzator
            if (c == '\n')
                return 2;
            else if (c == '+')
                return 3;
        }
    }
    modif(corect, primit, cuv);                  // se modifica vectorul cu codificarea
    color_row(5 + pas, poz - 2, corect, primit); // se coloreaza casutele in functie de codificare
    move(x + 4, poz);                            // se muta cursorul in prima casuta de pe randul urmator
    return verif(corect);                        // se returneaza 1 daca a fost ghicit cuvantul si 0 altfel
}
char start_joc(WINDOW *wnd, char cuv[7])
{
    int a, b, val = 0, i, e_corect, rez[7] = {0};
    char d;
    getmaxyx(wnd, a, b);     // se salveaza dimensiunea ecranului
    move(1, b / 2 - 3);      // mut cursorul pe centru
    attron(COLOR_PAIR(4));   // activez culoarea a 4 a
    printw(" WORDLE ");      // afisez titlul
    attroff(COLOR_PAIR(4));  // opresc culoarea
    draw(b / 2 - 20);        // desenez ecrnul jocului
    move(5, b / 2 - 20 + 3); // mut cursorul in prima casuta
    for (i = 0; i <= 5; i++)
    {
        e_corect = incercare(wnd, b / 2 - 20 + 3, cuv, rez, val); // sunt 6 sanse de a ghici cuvantul
        val += 4;                                                 // reprezinta linia pe care sunt introduse literele si se trece la urmatoarea
        if (e_corect == 1)                                        // daca s-a gasit cuvantul se opreste incercarea de a ghici
            break;
        else if (e_corect == 2) // daca se returneaza 2, jucatorul doreste sa renunte la joc
            return 'x';         // se returneaza ceva diferit de \n pentru a nu incepe un nou joc
        else if (e_corect == 3) // jucatorul doreste sa inceapa un joc nou
            return '\n';        // daca functia returneaza \n atunci va incepe un nou joc
    }
    if (e_corect)
    { // daca s-a gasit cuvantul se afiseaza mesajul corespunzator, de culoarea a 3-a
        move(2, b / 2 - 6);
        attron(COLOR_PAIR(3));
        printw(" FELICITARI! ");
        move(3, b / 2 - 20);
        printw(" Pentru un joc nou apasa tasta ENTER ");
        attroff(COLOR_PAIR(3));
        d = getch(); // se introduce un caracter, daca este \n va incepe un alt joc
        if (d == 58) // daca s-a apasat ':' apare meniul de control
        {
            attron(COLOR_PAIR(3)); // se activeaza culoarea cu numarul 3(rosu)
            move(2, b / 2 - 20);   // mergem la pozitia pentru prima optiune
            printw("Pentru a renunta la joc apasa tasta ENTER");
            move(3, b / 2 - 20 - 3); // pentru a doua
            printw("Pentru a incepe un joc nou apasa tasta + ");
            attroff(COLOR_PAIR(3)); // se opreste culoarea
            d = getch();            // se primeste un caracter caracterul cu semnificatia afisata
            if (d == '+')
                d = '\n';

            else
                d = '+';
        }
    }
    else
    { // daca nu s-a gasit cuvantul se afiseaza un alt mesaj
        move(2, b / 2 - 6);
        attron(COLOR_PAIR(4));
        printw(" Mai incearca! ");
        move(3, b / 2 - 20);
        printw(" Cuvantul corect este: ");
        for (i = 0; i <= 4; i++) // se afiseaza cuvantul corect, cu litere mari
            printw("%c", cuv[i] - 32);
        printw(" ");
        attroff(COLOR_PAIR(4));
        getch(); // se asteapta un alt caracter pentru a nu se inchide ecranul prea repede, altfel nu s-ar mai vedea mesajul
    }
    return d;
}
int main()
{
    int nr;
    char d, lista[28][6];
    WINDOW *wnd;
    wnd = initscr(); // initializez o fereastra
    start_color();   // initializez utilizarea culorilor, iar apoi perechile de culori pentru scris si fundal
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    creare_lista(lista); // creez lista din care se va alege cate un cuvant
    noecho();            // functia opreste repetarea literelor introduse
    d = '\n';            // d primeste enter pentru a intra in joc
    while (d == '\n')    // cand jucatorul castiga jocul ar optiunea de a apasa enter pentru un joc nou
    {                    // se elibereaza ecranul, se alege un cuvant nou si incepe jocul cat timp se apasa enter
        srand(time(NULL));
        nr = rand() % 27;              // se alege un numar de la 0 la cate cuvinte sunt
        clear();                       // se elibereaza fereastra
        d = start_joc(wnd, lista[nr]); // incepe un joc, d fiind ultima tasta apasata
    }
    endwin();
}