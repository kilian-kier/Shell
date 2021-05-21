# Shell - Kier Kilian


------------

## Installation und Ausführung

### help.o
help.o kann mit:
```bash
ld -r -b binary source/help.txt -o source/help.o
```
generiert werden.

### Windows

```cmd
windows.cmd
Shell.exe
```
oder
```cmd
cd source
ld -r -b binary help.txt -o help.o
gcc alias.c basic_cmd.c command.c file_cmd.c functions.c help.o input.c main.c struct.c -o ../Shell.exe
cd ..
Shell.exe
```
### Linux

```bash
./linux.sh
./shell
```
oder
```bash
make
./shell
```
oder
```bash
cd source
ld -r -b binary help.txt -o help.o
gcc alias.c basic_cmd.c command.c file_cmd.c functions.c help.o input.c main.c struct.c -o ../shell
cd ..
./shell
```

------------


## Befehle
Der Syntax der Befehle ist manchmal anders als wie es üblich wäre, da wenn mehrere Parameter übergeben werden können, 
ist der 1. Parameter immer fix und alle anderen Parameter können beliebig danach verwendet werden.

$: ein Parameter\
$$: beliebig viele Parameter\
($): optionaler Parameter

>**ls**\
ls ($): gib den Inhalt des übergebenen Ordners aus. Bei keinem Parameter den Inhalt des aktuellen Ordners.

>**cd**\
cd $: wechselt das current working directory in den übergebenen Pfad.

>**cat**\
cat $$: gibt den Inhalt der übergebenen Datei aus.

>**rm**\
rm $$: löscht die übergebenen Ordner oder Dateien. Deshalb kein rmdir notwendig

>**mv**\
mv $ $$: verschiebt die übergebene Dateien ($$) in den übergebenen Ordner ($)

>**cp**\
cp $ $$: kopiert den Inhalt der übergebene Dateien ($$) in die übergebene Datei ($). 
> Die Datei ($) wird überschrieben (nach Bestätigung des Benutzers) und falls noch nicht vorhanden wird sie erstellt. 
> Anstelle von einer Datei kann man auch einen Ordner ($) übergeben und die Dateien ($) dorthin kopieren. 
> Falls sich in diesem Ordner bereits eine Datei mit gleichen Name befindet wird sie überschrieben (nach Bestätigung des Benutzers).

>**echo**\
echo $$: gibt den übergebene Text aus.

>**mkdir**\
mkdir $$: erstellt im current working directory einen oder mehrere Ordner.

>**pwd**\
pwd: gibt das current working directory aus.

>**chmod**\
chmod $ $$: die Rechte der übergebene Dateinen und Ordner ($$) werden geändert. $ muss ein numerischer 
> Code sein mit der bereits in Linux bekannten Zahlenmethode.

>**touch**\
touch $$: erstellt im current working directory Dateien ($$).

>**help**\
help ($$): listet alle Befehle mit kurzer Beschreibung auf. Falls Parameter übergeben werden, wird nur die Beschreibung dieser Befehler aufgelistet.

>**exit**\
exit: beendet die Shell.


### zusätzliche Befehle
>**whoami**\
whoami: gibt den Username des Benutzers aus.

>**write**\
write ($) $ $$: schreibt $$ in die übergebene Datei $. 
> Standardmäßig wird überschrieben, aber mit -a ($) als ersten Parameter kann der übergeben String in die Datei angehängt werden.

>**date**\
date: gib das aktuelle Datum aus.

>**time**\
time: gib die aktuelle Uhrzeit aus.

>**set_alias**\
set_alias $ $$: für einen Befehl ($) können Aliase ($$) festgelegt werden, die in der Shell die gleiche Funktion aufrufen.

>**rm_alias**\
rm_alias $ $$: löscht die Aliase ($$) eines Befehls ($).

>**show_aliases**\
show_aliases: gibt alle aliase aus.


------------

## Kurze Beschreibung der Umsetzung
Beim Start werden die Befehle und dessen Beschreibung von der help.o Datei gelesen, 
die in das ausführende Programm eingebunden wurde, damit sie im Nachhinein nicht verändert werden kann. 
Die Aliase werden von einer Datei gelesen. Falls es diese Datei nicht gibt, wird eine neue erstellt.

Die Befehle werden in einen binären Baum gespeichert, der alphabetisch sortiert ist, um die Suche nach Befehlen zu vereinfachen. 
Jeder Knoten besitzt den Befehl als String, einen Zeiger auf die Funktion, die Beschreibung als String, 
einen Zeiger auf das erste Element des binären Baums für seine Aliase und jeweils einen Zeiger auf den linken und rechten Knoten im Baum.


Die Eingabe wird in ein struct gespeichert als String Array und Anzahl der Strings. 
Das erste Element des Arrays (der Befehl) wird im binären Baum gesucht und falls er nicht gefunden wurde, 
wird der Benutzer gefragt ob er ihn im System suchen und ausführen soll (mit system())

### was noch nicht funktioniert
 - Umlauten auf Windows (Ausgabe, Dateinamen, Write, …)
 - verschieben und kopieren von ganzen Ordnern
 - ls: wirkliche Größe auf Festplatte stimmt meistens nicht
 - rm: mehrere Ordner. Ein Ordner und mehrere Dateien funktioniert.