#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#CSV:xormos00

# CSV2XML
# Autor: Michal Ormos
# 4/2016

import argparse #parametry
import getopt #parametry
import sys #stdin, stdout, stderr
import re #regular expressions
import os #os.path.abspath
sys.path.append(sys.path.pop(0))
import csv

##
# Vypisanie napovedy
##
def napoveda():
    text="""
    CSV2XML
    -------
    IPP-proj2
    Michal Ormoš | xormos00@stud.fit.vutbr.cz
    3/2016 \t | FIT VUT Brno

    Options:
    [--help]
    [--input[=INPUT]] \t\tInput file [or STDIN]
    [--output[=OUTPU]] \t\tOutput file [or STDOUT]
    [-n] \t\t\tDisable generate XML Head
    [-r=root-element] \t\tElement cover output
    [-s=separator] \t\tRecord separator
    [-h[=SUBST] \t\tName of cols is subst or first row of data
    [-c=column-element]
    [l=line-element] \t\tCover every row of data
    [-i[=I]] \t\t\tPut index to every row
    [--start=START] \t\tIndex starts from START
    [-e] / [--error-recovery] \tBad count of cols are validate
    [--missing-field=VAL] \tMissing value is replace with VAL
    [--all-columns] \t\tWrite all colums
    [--padding]
    [--validate]
    """
    print(text)

##
# Funkcia na vytvaranie tabulator
# @PARAM tabSize urcije velkost tabulatora
# @PARAM xmlData vystupny stream
##
def tabulator(tabSize, xmlData):
    for i in range(0, tabSize):
        xmlData.write('    ')

##
# Funkcia pre tlac chyb a ukoncenie programu s chybovym kodom
# @PARAM errorString Chybove hlasenie
# @PARAM errorCode Chybovy kod
##
def printError(errorString, errorCode):
    sys.stderr.write(errorString)
    sys.exit(errorCode)

##
# Nahradzovanie neplatnych znakov povolenymi
# @PARAM string vstupny retazec urceny pre nahradenie nepovolenych znakov
##
def replace_bad_chars(string):
    string = re.sub('[&]', "&amp;", string)
    string = re.sub('[\"]', "&quot;", string)
    string = re.sub('[<]', "&lt;", string)
    string = re.sub('[>]', "&gt;", string)
    return string;
##
# Funkcia pre --padding. Prepocitava pocet stlpcov a riadkov celeho vstupenho suboru
# Upravuje piramo dane hodnoty a vracia ich spat do main
# @PARAM padd pocet riadkov v celom suboru
# @PARAM i aktualny riadok na vypisanie
# @RETURN Upravena hodnota pripravene pre vytlacenia na stdout
##
def order(padd, i):
    res1 = 0
    num1 = padd
    num2 = i
    while num1 > 0:
        num1 = int(num1 / 10)
        res1 += 1
    res2 = 0
    while num2 > 0:
        num2 = int(num2 / 10)
        res2 += 1
    diff = res1 - res2
    str(i)
    c = ''
    if (res1 > res2):
        c = '0'
        for z in range(1,diff):
            c += c
    c += str(i)
    return c

########################  MAIN  ########################
if __name__ == "__main__":

    # Citanie argumentov pomocou argparse
    parser = argparse.ArgumentParser(add_help=False)
    parser.add_argument('--input' , default=None)
    parser.add_argument('--output' , default=None)
    parser.add_argument('--help', nargs='?' , default=0)

    parser.add_argument('-n' , nargs='?', default=0)
    parser.add_argument('-r' , default=None)
    parser.add_argument('-s' , default=None)
    parser.add_argument('-h' , nargs='?', default=0)
    parser.add_argument('-c' , default=None)
    parser.add_argument('-l' , default=None)
    parser.add_argument('-i' , nargs='?', default=0)
    parser.add_argument('--start' , default=None)
    parser.add_argument('-e', '--error-recovery', nargs='?', default=0)
    parser.add_argument('--missing-field' , default=None)
    parser.add_argument('--all-columns' , nargs='?', default=0)
    parser.add_argument('--padding', nargs='?' , default=0)
    parser.add_argument('--validate', nargs='?' , default=0)

    args = parser.parse_args()

    # Priadania argumentov do priatelskejsich nazvov premennych
    input=args.input
    output=args.output
    negate_head=args.n
    root_element=args.r
    separator=args.s
    subst=args.h
    column_element=args.c
    line_element=args.l
    index=args.i
    start_n=args.start
    error_recovery=args.error_recovery
    missing_field_val=args.missing_field
    all_columns=args.all_columns
    padding=args.padding
    validate=args.validate

    # Osetrenie kolizie argumentov
    if (args.help == None):
        if (input != None or output != None or index == None or negate_head == None or error_recovery == None or subst == None or root_element != None or separator != None or line_element != None or start_n != None or missing_field_val != None or all_columns == None):
            printError("Chyba, parameter --help sa nesmie kombinovat so ziadnym inym", 1)

    if (validate != 0):
        if (index == None or negate_head == None or error_recovery == None or subst == None or root_element != None or separator != None or line_element != None or start_n != None or missing_field_val != None or all_columns == None):
            printError("Chyba, parameter --validate sa smie kombinovat len s parametrami --input a -- output", 1)

    if (index == None):
        if (line_element == None):
            printError("Chyba kombinace parametru, -i sa musi kombinovat s -l", 1)

    if (start_n != None):
        if (line_element == None or index != None):
            printError("Chyba kombinace parametru, --start sa musi kombinovat s -l a -i", 1)

    if (missing_field_val):
        if error_recovery==0:
            printError("Chyba kombinace parametru, --missing-field sa musi kombinovat s --error-recovery", 1)

    if (all_columns == None):
        if error_recovery==0:
            printError("Chyba kombinace parametru, --missing-field sa musi kombinovat s --error-recovery", 1)

    if (padding):
        if index==0:
            printError("Chyba kombinace parametru, --padding sa musi kombinovat s -i=", 1)

    # Pri --help vypis helo a program vypni
    if(args.help == None):
        napoveda()
        sys.exit(0)

    # Inicializacia inkrementalneho citaca
    indexCounter = 0
    if(index == None):
        index = 1
        indexCounter = 1

    # Presmerovanie na stdin
    if input==None:
        string = os.readlink('/proc/self/fd/0')
        string = re.sub('(.*\/)', "", string)
        input=string
    if input=="1":
        printError("Chyba, vstup bez parametrov", 1)

    # Kontrola platnych znakov pri -l
    if(line_element):
        if (re.search('.^[^\w:_]', line_element)
        or re.search('^[0-9]', line_element)
        or re.search('[^\w:_\.\-]/', line_element)):
            printError("Chybny nazev line_element parametru", 30)
        line_element = re.sub('[\"]', "", line_element)
    else:
        line_element='row';

    # Kontrola platnych znakov pri -r
    if(root_element):
        # @TODO
        if (re.search('^[0-9]', root_element)
        or re.search('^[^\w:_]', root_element)
        or re.search('[^\w:_.-]', root_element)):
            printError("Chybny nazev root_element parametru", 30)

    # Kontrola --start a nastavenie implicitnej hodnoty
    if(start_n):
        try:
            start_n = int(str(start_n))
        except:
            printError("Start element usi byt cislo!", 1)
        if(start_n < 0):
            printError("Chyba --start nesmi byt zaporni cislo", 1)
        index = start_n
        indexCounter = 1

    # Kontrola -s a nastavenie implicitnych hodnot
    if(separator == 'TAB'):
        separator = '\t'
    if(separator == '\\'):
        separator = '\\'
    if(separator == None):
        separator=",";
    if(separator == "\;"):
        separator=";";
    # V pripade, ze separator je zadany ako viacznakovy retazec
    if(len(separator) > 1):
        if (error_recovery):
            separator = '\n';
        else:
            printError("Chybne zadany parameter -s, je prilis dlhy", 1)

    # Kontrola -h a nastavenie implicitnej hodnoty
    if (subst == None):
        subst = '-'

    ############## OPENING OF INPUT FILE #######################
    csvData = 0
    try:
        csvData = csv.reader(open(os.path.abspath(input)), delimiter=separator)
    except IOError as e:
        printError("Chyba pri nacitani vstupneho suboru", 2)

    ############## OPENING OF OUTPUT FILE #######################
    xmlData = 0
    if (output):
        try:
            xmlData = open(output, 'w')
        except IOError as e:
            printError("Chybne opri nacitani vystupneho suboru", 3)
    else:
        xmlData = sys.stdout

    ############## Rozsirenie VLC #######################
    #nacitame data znova aby sme mohli ich prve validovat
    elementLen = 0
    if (validate != 0):
        try:
            copyCsvData = csv.reader(open(os.path.abspath(input)), delimiter=separator, quotechar='|')
        except IOError as e:
            printError("Chyba pri nacitani vstupneho suboru", 2)
        try:
            for data in copyCsvData:
                #Vsetky riadky by mali mat rovnaku dlzku
                if elementLen == 0:
                    elementLen = len(data)
                else:
                    if elementLen != len(data):
                        sys.exit(39)
                for element in data:
                    # Kontrola ci pred/za uvodzovkami nie je medzera
                    if (re.search('(\"\s)$', element)
                    or re.search('(\s\")', element)):
                        sys.exit(39)
                    #Ak je pred prvym alebo po poslednom elemente ciarka - chyba
                    #Ak je element hockjaky druh whitespace
                    if not element or element.isspace():
                        sys.exit(39)
        except:
            sys.exit(39)

    ############## Rozsirenie PAD #######################
    if(padding != 0):
        paddColNum = 0
        paddRowNum = 0
        try:
            copyCsvData = csv.reader(open(os.path.abspath(input)), delimiter=separator)
        except IOError as e:
            printError("Chyba pri nacitani vstupneho suboru", 2)
        for data in copyCsvData:
            paddRowNum += 1
            for element in data:
                if(paddRowNum == 1):
                    paddColNum += 1

    #####################################

    # Vypisanie hlavicky
    if (negate_head == 0):
        xmlData.write('<?xml version="1.0" encoding="UTF-8"?>' + "\n")


    # Inicializacia potrebnych premennych
    tabSize = 0 # vekost odriadkovania
    if(root_element):
        xmlData.write('<%s>\n' % (root_element,))
        tabSize +=1
    number_of_row = 0 # pocet riadkov v subore, pre @param subst (-h)
    rowNum = 0 # pocitadlo riadkov
    colNum = 1 # pocitadlo stlpcov
    max_row_by_header = 0 #pocet riadkov v hlavicke
    row_except = "" #for missing val parameter
    defaultCols = 0 # nastavenie implicitnej hodnoty poctu stlpcov

##
# Main core
# Spracuvava vstupny subor a vzhladom na vstupne parametry postupne vytvara xml
# Rozdeleny na 2 casti podla parametru -h
# V pripade, ze je zadany prvy riadok sluzi ako hlavicka, inak su hlavicky xml
# @NOTE POZOR subor pomocou row in csvData sa da prechadzat len raz
# @FUNC tabulator(tabSize, xmlData)
# @param tabSize riadia odriadkovanie XML suboru
##
    # Pri parametri -h je prvy riadok nazovom row elementov
    if (subst):
        for row in csvData:
            number_of_row += 1
            # Prvy riadok nacitame ako nazvy stlpcov
            if rowNum == 0:
                tags = row
                # Nahradzuje medzery a w/ v nazvov stlpcoch
                for i in range(len(tags)):
                    tags[i] = tags[i].replace(' ', subst)
                    tags[i] = tags[i].replace(',', subst)
                    tags[i] = tags[i].replace('\n', subst + subst)
                    tags[i] = tags[i].replace('\r', subst + subst)
                    if re.search('^[\d-].*$', tags[i]):
                        printError("Chybny nazev stlpca", 31)
            # Ostatne riadky vypisujeme ako jednotlivy riadky
            else:
                # Ak si uzivatel zvolil vlastny nazov riadku v elemente
                if (line_element):
                    tabulator(tabSize, xmlData)
                    if (indexCounter):
                        if (padding != 0):
                            xmlData.write('<%s index="%s">\n' % (line_element, order(paddRowNum, index,),))
                        else:
                            xmlData.write('<%s index="%d">\n' % (line_element, index,))
                    else:
                        xmlData.write('<%s>\n' % (line_element,))
                    tabSize +=1
                else:
                    tabulator(tabSize, xmlData)
                    if (indexCounter):
                        if (padding != 0):
                            xmlData.write('<row index="%s">\n' % order(paddRowNum, index,))
                        else:
                            xmlData.write('<row index="%d">\n' % (index,))
                    else:
                        xmlData.write('<row>' + "\n")
                    tabSize +=1
                # V nekorektnom pripade prisposob pocet riadkov
                if ((missing_field_val) or (all_columns == None)):
                    if len(row) > len(tags):
                        defaultCols = row
                    else:
                        defaultCols = tags
                else:
                    if(defaultCols == 0):
                        defaultCols = tags
                # Vypisovanie riadkov
                for i in range(len(defaultCols)):
                    # Ak je zapnuty prepinac --missing-field dorovnaj pocet stlpcov
                    try:
                        row[i] = replace_bad_chars(row[i])
                    except IndexError:
                        if (missing_field_val):
                            row_except = replace_bad_chars(missing_field_val)
                    tabulator(tabSize, xmlData)
                    try:
                        xmlData.write('<' + tags[i] + '>\n')
                    except:
                        if (error_recovery != None):
                            printError("Prazdne riadky nepovolene", 32)
                        else:
                            if (column_element):
                                xmlData.write('<' + column_element + str(i+1) + '>\n')
                            else:
                                xmlData.write('<col' + str(k+2) + '>\n')
                    tabSize +=1
                    tabulator(tabSize, xmlData)
                    if (row_except):
                        xmlData.write(row_except + "\n")
                    else:
                        try:
                            xmlData.write(row[i] + "\n")
                        except:
                            if (error_recovery != None):
                                printError("Prazdne riadky nepovolene", 32)
                            else:
                                xmlData.write(row_except + "\n")
                    tabSize -=1
                    tabulator(tabSize, xmlData)
                    try:
                        xmlData.write('</' + tags[i] + '>\n')
                    except:
                        if (error_recovery != None):
                            printError("Prazdne riedky nepovolene", 32)
                        else:
                            if (column_element):
                                xmlData.write('</' + column_element + str(i+1) + '>\n')
                            else:
                                xmlData.write('</col' + str(k+2) + '>\n')
                    colNum += 1
                    if (i == len(defaultCols)-1):
                        row_except = ""
                    k = i
                tabSize -=1
                tabulator(tabSize, xmlData)
                if (line_element):
                    xmlData.write('</%s>\n' % (line_element,))
                else:
                    xmlData.write('</row>' + "\n")
                if (indexCounter):
                    index +=1
            rowNum +=1
        if (number_of_row <= 1):
            printError("Header leading to invalid element (starts with a dash)", 31)
    # Ak nie je zadane -h subor sa prechadza implicitne
    # Podobne ako v prvej casti
    for row in csvData:
        if (line_element):
            tabulator(tabSize, xmlData)
            if (indexCounter):
                if (indexCounter):
                    if (padding != 0):
                        xmlData.write('<%s index="%s">\n' % (line_element, order(paddRowNum, index,),))
                    else:
                        xmlData.write('<%s index="%d">\n' % (line_element, index,))
            else:
                xmlData.write('<%s>\n' % (line_element,))
            tabSize +=1
        else:
            tabulator(tabSize, xmlData)
            if (indexCounter):
                if (padding != 0):
                    xmlData.write('<row index="%s">\n' % (order(paddRowNum, index,)))
                else:
                    xmlData.write('<row index="%d">\n' % (index,))
            else:
                xmlData.write('<row>' + "\n")
            tabSize +=1
        colNum = 1
        tags = row
        if ((missing_field_val) or (all_columns == None)):
            if (defaultCols == 0):
                defaultCols = row
            if(len(row) > len(defaultCols)):
                tags = row
            else:
                tags = defaultCols
        elif (error_recovery == None):
            if (defaultCols == 0):
                defaultCols = row
            else:
                tags = defaultCols
        else:
            if (defaultCols == 0):
                defaultCols = row
        if ((error_recovery != None) and (len(row) > len(defaultCols))):
            printError("Prilis vela stlpcov, neriesim", 32)
        for i in range(len(tags)):
            # V pripade danych prepinacov nevypisuj prebytocne stlpce
            if(rowNum > 0 and colNum > max_row_by_header and error_recovery == None):
                if(all_columns != None):
                    break
            tabulator(tabSize, xmlData)
            if (padding != 0):
                xmlData.write('<col%s>\n' % (order(paddColNum, colNum),))
            else:
                xmlData.write('<col%d>\n' % (colNum,))
            tabSize +=1
            tabulator(tabSize, xmlData)
            try:
                row[i] = replace_bad_chars(row[i])
                xmlData.write(row[i] + "\n")
            except:
                if (error_recovery != None):
                    printError("Prazdne riadky nepovolene", 32)
                else:
                    if ((missing_field_val) or (all_columns == None)):
                        missing_field_val_temp = replace_bad_chars(missing_field_val)
                        xmlData.write(missing_field_val_temp + "\n")
            tabSize -=1
            tabulator(tabSize, xmlData)
            if (padding != 0):
                xmlData.write('</col%s>\n' % (order(paddColNum, colNum),))
            else:
                xmlData.write('</col%d>\n' % (colNum,))
            colNum += 1
            if (rowNum == 0):
                max_row_by_header +=1
                if (column_element and len(row) > len(tags)):
                    colNum = k + i
                    for k in range(len(tags)-1):
                        k += i + 1
                        # Ak je zapnuty prepinac XXX dorovnaj pocet stlpcov
                        try:
                            row[k] = replace_bad_chars(row[k])
                        except IndexError:
                            row_except = replace_bad_chars(missing_field_val)
                        tabulator(tabSize, xmlData)
                        xmlData.write('<' + column_element + str(int(colNum)) + '>\n')
                        tabSize +=1
                        tabulator(tabSize, xmlData)
                        if (row_except):
                            xmlData.write(row_except + "\n")
                        else:
                            xmlData.write(row[k] + "\n")
                        tabSize -=1
                        tabulator(tabSize, xmlData)
                        xmlData.write('</' + column_element + str(int(colNum)) + '>\n')
                        colNum += 1
                        if (row_except):
                            row_except = 0
                            break
        tabSize-= 1
        tabulator(tabSize, xmlData)
        if (line_element):
            xmlData.write('</%s>\n' % (line_element,))
        else:
            xmlData.write('</row>' + "\n")
        rowNum +=1
        if (indexCounter):
            index +=1

    if(root_element):
        xmlData.write('</%s>\n' % (root_element,))
try:
    xmlData.close()
except IOError as e:
    printError("Nepodarilo sa uzavriet vystupny subor", 4)
