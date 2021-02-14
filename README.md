# Jednoduchý textový editor
Jednoduchý textový editor. Vstupní text je čten ze standardního vstupu a příkazy pro úpravu textu jsou uloženy v souboru, který je předán jako 1. argument programu. Projekt je vytvořen do předmětu [Základy programování](https://www.fit.vut.cz/study/course/14047/.cs) na [FIT VUT](https://www.fit.vut.cz/.cs).

## Instalace (Linux)
Stáhněte si repozitář a pro vytvoření spustitelného souboru využijte přiložený Makefile:
```
git clone https://github.com/TomasBeranek/IZP-project-1.git && cd IZP-project-1
make
```
Pro vyčištění projektu:
```
make clean
```

## Příkazy
Každý příkaz je uveden na samostatném řádku a může být jeden z:

``` bCONTENT ``` -- před aktuální řádek přidá CONTENT

``` iCONTENT ``` -- před aktuální rádek vloží řádek s obsahem CONTENT

``` dN ``` -- vymaže N aktuálních rádků

``` r ``` -- odstraní znak konce řádku

``` nN ``` -- vytiskne N aktuálních řádků

``` q ``` -- ukončí editaci (program)

``` aCONTENT ``` -- za aktuální řádek přidá CONTENT

``` gX ``` -- přeskočí na X-tý příkaz pro editaci

``` e ``` -- přidá na konec aktuálního řádku odřádkování ('\n')

## Příklad použití
Obsah souboru ```commands.txt```:
```
bHello 
a!
iFirst line
q
```

Obsah souboru ```input```:
```
world
```

Spuštění editoru:
```
./editor commands.txt < input
First line
Hello world!
```

## Licence
MIT License
