/*
 * Soubor:       Makefile
 * Predmet:      Zaklady programovani (IZP) -- FIT VUT
 * Projekt:      Jednoduchý textový editor
 * Autor:        Tomas Beranek (xberan46)
 * Email:        xberan46@stud.fit.vutbr.cz
 * Datum:        25.10.1018
 *
 * Popis:        Jednoduchy textovy editor, ktery nacita vstupni text ze stdin a prikazy pro upravu
 *               textu ze souboru, ktery je predan jako 1. argument programu
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHARS_ON_LINE 1010

enum err
{
            SUCCESS = 0,
            ERR_FILE_NOT_OPENED = -1,
            ERR_FILE_NOT_CLOSED = -2,
            ERR_WRONG_FORMAT_OF_COMMAND = 1,
            ERR_UNKNOWN_COMMAND = 2,
};

void command_b(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE]);
void command_i(char command[CHARS_ON_LINE]);
void command_r(char input[CHARS_ON_LINE]);
int command_q(char input[CHARS_ON_LINE], FILE* f_command, unsigned int change);
void command_a(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE]);
int command_g(FILE* f_command, char file_name[100], char command[CHARS_ON_LINE]);
void print_all(char input[CHARS_ON_LINE]);
int command_n(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE], FILE* f_command);
int command_d(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE], FILE* f_command);


int main(int argc, char *argv[])
{
        //pokud je pocet argumentu mene nebo vice nez 2, program vypise svuj popis a ukonci se
    if (argc !=2 )
    {
        printf("Zadejte prave 1 argument s adresou souboru s prikazy!\n");
        return SUCCESS;
    }

        //otevreni souboru s prikazy a kontrola otevreni
    FILE *f_command = fopen(argv[1], "r");
    if (f_command == NULL)
    {
        fprintf(stderr, "ERR_FILE_NOT_OPENED\n");
        return ERR_FILE_NOT_OPENED;
    }

    char input[CHARS_ON_LINE];      //aktualni radek
    char command[CHARS_ON_LINE];    //aktualni prikaz
    int result;     //informace o zpusobu ukonceni funkce
    unsigned int change = 0;	    //informace o zmene aktualniho radku
        //vymazani retezcu command a input
    memcpy(command, "", 2);
    memcpy(input, "", 2);

        //nacteni prvniho radku ze vstupu, pokud neni zadan, ukonci program
    if ((fgets(input, CHARS_ON_LINE, stdin)) == NULL)
    {
            //zavreni souboru s prikazy a overeni zavreni
        if (fclose(f_command) == EOF)
        {
            fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
            return ERR_FILE_NOT_CLOSED;
        }
        return SUCCESS;
    }

         //nacita radky s prikazy dokud nedojde na konec
    while ((fgets(command, CHARS_ON_LINE, f_command)) != NULL)
    {
            //pokud neni po prikazu odrakovani, prida jej
        if (command[strlen(command) - 1] != '\n')
                command[strlen(command)] = '\n';

            //podle 0. znaku z command rozlisuje jednotlive prikazy
        switch (command[0])
        {
                //prikaz "bCONTENT", pred aktualni radek prida CONTENT
            case 'b':
                command_b(input, command);
		change++;
                break;

                    //prikaz "iCONTENT", pred aktualni radek vlozi radek s obsahem CONTENT
            case 'i':
                command_i(command);
		change++;
                break;

                    // prikaz "dN" vymaze N aktualnich radku
            case 'd':
                result = command_d(input, command, f_command);
                if (result != 10)
                    switch (result)
                    {
                        case SUCCESS:
                            return SUCCESS;
                            break;
                        case ERR_WRONG_FORMAT_OF_COMMAND:
                            fprintf(stderr, "ERR_WRONG_FORMAT_OF_COMMAND\n");
                            return ERR_WRONG_FORMAT_OF_COMMAND;
                            break;
                        case ERR_FILE_NOT_CLOSED:
                            fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
                            return ERR_FILE_NOT_CLOSED;
                            break;
                    }
		change = 0;
                break;

                //prikaz "r" odstrani znak konce radku
            case 'r':
                command_r(input);
		change++;
                break;

                //prikaz "nN" vytiskne N aktualnich radku
            case 'n':
                result = command_n(input, command, f_command);
                if (result != 10)
                    switch (result)
                    {
                        case SUCCESS:
                            return SUCCESS;
                            break;
                        case ERR_WRONG_FORMAT_OF_COMMAND:
                            fprintf(stderr, "ERR_WRONG_FORMAT_OF_COMMAND\n");
                            return ERR_WRONG_FORMAT_OF_COMMAND;
                            break;
                        case ERR_FILE_NOT_CLOSED:
                            fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
                            return ERR_FILE_NOT_CLOSED;
                            break;
                    }
		change = 0;
                break;

                //prikaz "q" ukonci editaci (program)
            case 'q':
                return command_q(input, f_command, change);
                break;

                //prikaz "aCONTENT" za aktualni radek prida CONTENT
            case 'a':
                command_a(input, command);
		change++;
                break;

                //prikaz "gX" preskoci s aktualnim prikazem pro editaci na Xty prikaz
            case 'g':
                {
                        //pokud command_q vrati neco jineho nez 0, program se ukonci s prislusnou chybovou hlaskou a jejim kodem
                    result = command_g(f_command, argv[1], command);
                    if (result != 0)
                        switch (result)
                        {
                            case ERR_WRONG_FORMAT_OF_COMMAND:
                                fprintf(stderr, "ERR_WRONG_FORMAT_OF_COMMAND\n");
                                return ERR_WRONG_FORMAT_OF_COMMAND;
                                break;

                            case ERR_FILE_NOT_CLOSED:
                                fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
                                return ERR_FILE_NOT_CLOSED;
                                break;

                            case ERR_FILE_NOT_OPENED:
                                fprintf(stderr, "ERR_FILE_NOT_OPENED\n");
                                return ERR_FILE_NOT_OPENED;
                                break;
                        }
                }
                break;

                //prikaz "e" prida na konec aktualniho radku '\n'
            case 'e':
                input[strlen(input)] = '\n';
                change++;
		break;

                //pri nacteni neznameho prikazu vypise chybovou hlasku s prislusnym kodem
            default:
                fprintf(stderr, "ERR_UNKNOWN_COMMAND\n");
                return ERR_UNKNOWN_COMMAND;
                break;

            }
            //pred novym nactenim retezce command jej vymaze
        memcpy(command, "", 2);
    }
        //vytiskne zbyvajici radky ze vstupu
    print_all(input);

        //zavreni souboru s prikazy a overeni zavreni
    if (fclose(f_command) == EOF)
    {
       fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
       return ERR_FILE_NOT_CLOSED;
    }

    return SUCCESS;
}

void print_all(char input[CHARS_ON_LINE])
{
        //vytiskne aktualni radek
    printf("%s", input);
        //tiskne zbyvajici radky
    while(fgets(input, CHARS_ON_LINE, stdin) != NULL)
    {
        printf("%s", input);
    }
        //pokud neni input ukoncen znakem '\n', prida jej
    if (input[strlen(input) - 1] != '\n')
        printf("\n");

}

void command_b(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE])
{
    char content[CHARS_ON_LINE];
    char copy[CHARS_ON_LINE];

        //vymazani retezcu input a copy
    memcpy(content, "", 2);
    memcpy(copy, "", 2);

        //nacte command bez prvniho posledniho znaku do retezce content
    unsigned int i = 1;
    while (command[i] != '\n')
    {
        content[i-1] = command[i];
        i++;
    }

        //do content nacita znaky  z command od druheho do predposledniho, zbyle nastavi na '\0'
    for (i = 1; i < CHARS_ON_LINE; i++)
    {
        if (i <= (strlen(command) - 3))
            content[i - 1] = command[i];
        else
            content[i] = '\0';
    }

        //pocet znaku v retezci input po ukonceni prikazu "bCONTENT"
    unsigned count = strlen(content) + strlen(input);

        //zkopiruje nejdrive content a potom input do copy
    for (i = 0; i < count; i++)
    {
        if (i < strlen(content))
        {
            copy[i] = content[i];
        }
        else
        {
            copy[i] = input[i - strlen(content)];
        }
    }

        //vymazani retezce input
    memcpy(input, "", 2);

       //zkopiruje copy do input
    for (i = 0; i < CHARS_ON_LINE; i++)
    {
        if (i < count)
        {
            input[i] = copy[i];
        }
        else
        {
            input[i] = '\0';
        }
    }
    return;
}

void command_i(char command[CHARS_ON_LINE])
{
        //vypise command bez prvniho znaku na standartni vystup
    unsigned int i = 1;
    while (command[i] != '\0')
        {
            printf("%c", command[i]);
            i++;
        }
    return;
}

void command_r(char input[CHARS_ON_LINE])
{
    if (input[strlen(input) - 1] == '\n')    //pokud je posledni znak '\n'
        input[strlen(input) - 1] = '\0';        //posledni znak nastavi na '\0'
    return;
}

int command_q(char input[CHARS_ON_LINE], FILE* f_command, unsigned int change)
{
    if (input[strlen(input) - 1] != '\n')
	input[strlen(input)] = '\n'; 
   
        //pokud byla provedena zmena retezce input, tak jej vytiskne 
    if (change != 0)
 	printf("%s", input);
        //zavreni souboru s prikazy, pri chybnem zavreni vraci chybovou hlasku s prislusnym kodem, jinak se uspesne ukonci   
    if (fclose(f_command) == EOF)
    {
        fprintf(stderr, "ERR_FILE_NOT_CLOSED\n");
        return ERR_FILE_NOT_CLOSED;
    }
    return SUCCESS;
}

void command_a(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE])
{
    char content[CHARS_ON_LINE];
    char copy[CHARS_ON_LINE];

        //vymazani retezcu content a copy
    memcpy(content, "", 2);
    memcpy(copy, "", 2);

        //nacte command bez prvniho a posledniho znaku do content
    unsigned int i = 1;
    while (command[i] != '\n')
    {
        content[i-1] = command[i];
        i++;
    }

        //znaky ktere nepatri do content nastavi na '\0'
    for  (i = 0; i < CHARS_ON_LINE; i++)
    {
        if (i > (strlen(command) - 3))
        content[i] = '\0';
    }

        //pokud aktualni radek obsahuje konec radku c=0, jinak c=1
    int c;
    if (input[strlen(input) - 1] == '\n')
        c = 0;
    else
        c = 1;

        //pocet znaku v obou retezcich i s odrakovanim
    unsigned int count = strlen(content) + strlen(input) + c;

        //zkopiruje nejdrive input a potom content do copy
    for (i = 0; i < (count - 1); i++)
    {
        if (i < (strlen(input) - 1 + c))
        {
            copy[i] = input[i];
        }
        else
        {
            copy[i] = content[i - (strlen(input) - 1 + c)];
        }
    }

        //vymazani input
    memcpy(input, "", 2);

        //zkopiruje copy do input
    for (i = 0; i < CHARS_ON_LINE; i++)
    {
            //kopiruje copy do input
        if (i < count - 1)
        {
            input[i] = copy[i];
        }
        else
        {
                //na konec prida znak '\n'
            if (i == (count -1))
            {
                input[i] = '\n';
            }
                //zbytek  nastavi na '\0'
            else
            {
                input[i] = '\0';
            }
        }
    }
    return;
}

int command_g(FILE* f_command, char file_name[100], char command[CHARS_ON_LINE])
{
    unsigned int count = 0;
    unsigned int i;

        //pokud command neobsahuje cislo nebo cislo zacina '0' vypise chybovou hlasku s prislusnym kodem
    if ((strlen(command) <= 2) || (command[1] == '0'))
        return ERR_WRONG_FORMAT_OF_COMMAND;

        //prevede cislo z retezce na int a ulozi jej do count
    for ( i = 1; command[i] != '\n'; i++)
    {
        if (((command[i] - 48) < 0) && ((command[i] - 48) > 9))         //pokud N neni tvoreno z cislic, vypise chybovou hlasku a prislusny kod
        {
            return ERR_WRONG_FORMAT_OF_COMMAND;
        }
        count = 10*count + (command[i] - 48);           //prevod cislic z retezce na cislo
    }

        //zavreni souboru s prikazy a overeni zavreni
    if (fclose(f_command) == EOF)
        return ERR_FILE_NOT_CLOSED;

        //otevreni souboru s prikazy a kontrola otevreni
    if ((f_command  = fopen(file_name, "r")) == NULL)
        return ERR_FILE_NOT_OPENED;

        //nacita (count - 1)krat prikaz a tim jej premaze
    if (count != 1)
        for (i = 0; i < (count - 1); i++)
        {
            memcpy(command, "", 2);
            if (fgets(command, CHARS_ON_LINE, f_command) == NULL)
                return ERR_WRONG_FORMAT_OF_COMMAND;
        }
    return 0;
}

int command_n(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE], FILE* f_command)
{
    unsigned int count = 0;
    unsigned int i;

        //pokud N zacina '0' program se ukonci
    if (command[1] == '0')
        return ERR_WRONG_FORMAT_OF_COMMAND;

        //pokud prikaz neobsahuje N tak count = 1
    if (strlen(command) <= 2)
    {
        count = 1;
    }

        //pokud prikaz obsahuje N, prevede jej na cislo a ulozi do count
    else
    {
        for (i = 1; command[i] != '\n'; i++)
        {
            if (((command[i] - 48) < 0) && ((command[i] - 48) > 9))         //testovani zda je N je tvoreno z cislic
                return ERR_WRONG_FORMAT_OF_COMMAND;
            count = 10*count + (command[i] - 48);           //prevod cislic z retezce na cislo
        }
    }

        //Nkrat opakuje vypsani a nacteni aktualniho radku
    for (i = 0; i < count; i++)
    {
        printf("%s", input);
            //pred nactenim retezce input jej vymaze
        memcpy(input, "",  1);
        if (fgets(input, CHARS_ON_LINE, stdin) == NULL)         //pokud je soubor prazdny ukonci editaci
            {
                    //zavreni souboru s prikazy a overeni zavreni
                if (fclose(f_command) == EOF)
                    return ERR_FILE_NOT_CLOSED;
                return SUCCESS;
            }
    }
        //vraci jinou hodnotu nez jsou chybove kody
    return 10;
}

int command_d(char input[CHARS_ON_LINE], char command[CHARS_ON_LINE], FILE* f_command)
{
        //pocet preskocenych radku - count
    unsigned int count = 0;
    unsigned int i;

        //pokud N zacina '0' program se ukonci
    if (command[1] == '0')
    return ERR_WRONG_FORMAT_OF_COMMAND;

        //pokud prikaz neobsahuje N tak count = 1
    if (strlen(command) <= 2)
    {
        count = 1;
    }

        //pokud prikaz obsahuje N, prevede jej na cislo a ulozi do count
    else
    {
        for (i = 1; command[i] != '\n'; i++)
        {
            if (((command[i] - 48) < 0) && ((command[i] - 48) > 9))         //testovani zda je N je tvoreno z cislic
                return ERR_WRONG_FORMAT_OF_COMMAND;
            count = 10*count + (command[i] - 48);           //prevod cislic z retezce na cislo
        }
    }

        //Nkrat opakuje nacteni aktualniho radku
    for (i = 0; i < count; i++)
    {
        memcpy(input, "",  1);
        if (fgets(input, CHARS_ON_LINE, stdin) == NULL)         //pokud je soubor prazdny ukonci editaci
        {
                //zavreni souboru s prikazy a overeni zavreni
            if (fclose(f_command) == EOF)
                return ERR_FILE_NOT_CLOSED;
            return SUCCESS;
        }
    }
    return 10;
}
