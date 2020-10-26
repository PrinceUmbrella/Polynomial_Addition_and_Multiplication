#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

/*==========================================
            CONSTANTS
===========================================*/

const int MIN_NUMBER_OF_INPUT = 2;
const int MAX_NUMBER_OF_INPUT = 12;

/*==========================================
            STRUCT
===========================================*/
struct ATerm
{
    int Coefficient;
    int Exponent;
    ATerm *Next;
};

typedef ATerm *Polynomial;

/*==========================================
            Function Prototypes
===========================================*/

int getNumberOfPolynomials();
bool checkTerms(string polyTerm);
bool checkInput(string numberOfPolynomials);
void printList(Polynomial current);
string getUserInput(int inputNumber);
string removeSign(string userCoeff);
string removeSpaces(string polyTerms);
int getNumberOfInput(string userInput);
bool isNumberOfInput(string userInput);
string *getCoeffecient(string polyTerm);
bool checkParsedData(string dataParsed);
bool differentCharFound(string userInput);
int getNumberOfTerms(string polyFunction);
bool checkExponentsOrder(string polyFunction);
string getExponent(string polyTerm, int startPosition);
string *getTerms(string polyFunction, int numberOfTerms);
void passToList(string polynomialTerm, Polynomial &head);
Polynomial addition(Polynomial headOne, Polynomial headTwo);
void termsToLinkedList(string polyFunction, Polynomial &head);
Polynomial multiplyWithTerm(Polynomial headOne, Polynomial term);
Polynomial multiplyPolynomial(Polynomial headOne, Polynomial headTwo);
void addtoList(Polynomial &head, int inputCoeffecient, int inputExponent);
void finalPrint(Polynomial *headArray, char sign, Polynomial typeOfFunction, int numberOfPolynomial);

int main()
{
    int numberOfPolynomial;
    string *polyFunctions, userInput;
    Polynomial *headArray;

    Polynomial sum, product;

    sum = nullptr;
    product = nullptr;

    numberOfPolynomial = getNumberOfPolynomials();
    headArray = nullptr;
    headArray = new Polynomial[numberOfPolynomial];

    for (int i = 0; i < numberOfPolynomial; i++)
    {
        userInput = getUserInput(i);
        headArray[i] = nullptr;
        termsToLinkedList(userInput, headArray[i]);
    }

    for (int i = 0; i < numberOfPolynomial; i++)
    {
        sum = addition(sum, headArray[i]);
    }

    product = headArray[0];
    for (int i = 1; i < numberOfPolynomial; i++)
    {
        product = multiplyPolynomial(product, headArray[i]);
    }
    cout << endl;

    finalPrint(headArray, '+', sum, numberOfPolynomial);
    cout << endl;
    finalPrint(headArray, '*', product, numberOfPolynomial);
    cout << endl;
}

/*=========================================================
            Gets the number of polynomials the user wants
            checks and returns the final answer
===========================================================*/

int getNumberOfPolynomials()
{
    string numberOfPoly;

    cout << " Number of polynomials -> ";
    getline(cin, numberOfPoly);

    while (checkInput(numberOfPoly))
    {
        cout << " Incorrect Input !!!" << endl;
        cout << " Enter Number of polynomials again -> ";
        getline(cin, numberOfPoly);
    }

    return stoi(numberOfPoly);
}

/*=================================================
    Checks if the number of polynomials input has
    invalid data. And returns a boolean
=================================================*/

bool checkInput(string numberOfPolynomials)
{
    bool isCorrect;

    isCorrect = false;

    for (int i = 0; i < numberOfPolynomials.length(); i++)
    {
        if (numberOfPolynomials[i] < '0' ||
            numberOfPolynomials[i] > '9')
        {
            isCorrect = true;
        }
    }

    if (numberOfPolynomials.length() == 0 ||
        numberOfPolynomials.length() > 2)
    {
        isCorrect = true;
    }

    if (!isCorrect)
    {
        if (stoi(numberOfPolynomials) < MIN_NUMBER_OF_INPUT ||
            stoi(numberOfPolynomials) > MAX_NUMBER_OF_INPUT)
        {
            isCorrect = true;
        }
    }

    return isCorrect;
}

/*==============================================
    Gets the user input number then returns
    the user input as long as the user enters
    in the correct format.
===============================================*/

string getUserInput(int inputNumber)
{
    string userInput;
    bool isCorrect = false;

    cout << " Input polynomial " << inputNumber + 1 << " -> ";
    getline(cin, userInput);
    userInput = removeSpaces(userInput);

    if (userInput.length() != 0 && !differentCharFound(userInput))
    {
        isCorrect = checkTerms(userInput);
    }
    while (!isCorrect)
    {
        cout << endl;
        cout << " Please Incorrect Polynomial Input" << endl;
        cout << " Please Enter Input " << inputNumber + 1 << " again ->";
        getline(cin, userInput);
        userInput = removeSpaces(userInput);
        if (userInput.length() != 0)
        {
            isCorrect = checkTerms(userInput);
        }
    }

    return userInput;
}

/*======================================================
    Creates and returns an array of terms
========================================================*/

string *getTerms(string polyFunction, int numberOfTerms)
{
    string *termsArray;
    int termPosition;
    termPosition = 0;
    termsArray = new string[numberOfTerms];
    string term = "";

    for (int i = 0; i < polyFunction.length(); i++)
    {
        if (polyFunction[i] == '+')
        {
            termsArray[termPosition] = term;
            term = "";
            termPosition++;
        }
        else if (polyFunction[i] == '-' && polyFunction[i - 1] != '^' && i != 0)
        {
            termsArray[termPosition] = term;
            term = "-";
            termPosition++;
        }
        else
        {
            term += polyFunction[i];
        }
    }
    termsArray[termPosition] = term;

    return termsArray;
}

/*======================================================
    Creates an array of linked lists for the string
    polynomial given.
========================================================*/

void termsToLinkedList(string polyFunction, Polynomial &head)
{
    string *termsArray;
    int numberOfTerms;

    numberOfTerms = getNumberOfTerms(polyFunction);
    termsArray = getTerms(polyFunction, numberOfTerms);

    for (int i = 0; i < numberOfTerms; i++)
    {
        passToList(termsArray[i], head);
    }
}

/*======================================================
    passes each term from the linked list to the array
========================================================*/

void passToList(string polynomialTerm, Polynomial &head)
{
    string *finalInput;
    string finalInputExp;
    int inputCoeffecient;
    int inputExponent;

    finalInput = getCoeffecient(polynomialTerm);
    inputCoeffecient = stoi(finalInput[0]);
    finalInputExp = getExponent(polynomialTerm, stoi(finalInput[1]));
    inputExponent = stoi(finalInputExp);

    addtoList(head, inputCoeffecient, inputExponent);
}

/*======================================================
    Extracts the coeffcient from the term
========================================================*/

string *getCoeffecient(string polyTerm)
{
    string coeffecient = "";
    string *finalReturn;
    int place = 0;
    bool noCoeffecient = true;

    finalReturn = new string[2];

    while (polyTerm[place] != 'x' &&
           polyTerm[place] != 'X' &&
           place != polyTerm.length())
    {
        coeffecient += polyTerm[place];
        place++;
        noCoeffecient = false;
    }

    if (noCoeffecient)
    {
        coeffecient = "1";
    }

    if (coeffecient == "-")
    {
        coeffecient = "-1";
    }

    finalReturn[0] = coeffecient;
    finalReturn[1] = to_string(place);

    return finalReturn;
}

/*======================================================
    Extracts the exponent from the term
========================================================*/

string getExponent(string polyTerm, int startPosition)
{
    string exponent = "";
    int place = startPosition;
    bool xPosition;
    bool noTerm = true;

    xPosition = (polyTerm[place] != 'x' && polyTerm[place] != 'X');
    // 
    if (place < polyTerm.length() - 1)
    {
        place = place + 2;
        while (place != polyTerm.length())
        {
            exponent += polyTerm[place];
            place++;
            noTerm = false;
        }
    }

    if (noTerm && xPosition)
    {
        exponent = "0";
    }
    else if (!xPosition && noTerm)
    {
        exponent = "1";
    }
    return exponent;
}

/*======================================================
    Adds a term to a list
========================================================*/

void addtoList(Polynomial &head, int inputCoeffecient, int inputExponent)
{
    Polynomial temp, current;

    current = head;
    temp = nullptr;
    temp = new ATerm;
    temp->Next = nullptr;

    temp->Coefficient = inputCoeffecient;
    temp->Exponent = inputExponent;

    if (!head)
    {
        head = temp;
    }
    else
    {

        while (current->Next != nullptr)
        {
            current = current->Next;
        }

        current->Next = temp;
    }
}

/*==========================================
            Prints the Linked List
===========================================*/

void printList(Polynomial current)
{
    if (current)
    {
        cout << "Coefficient: " << current->Coefficient << endl;
        cout << "Exponent: " << current->Exponent << endl;
        printList(current->Next);
    }
}

Polynomial addition(Polynomial headOne, Polynomial headTwo)
{
    Polynomial finalSum, currentOne, currentTwo;
    int sumCoeffcient;

    finalSum = nullptr;
    currentOne = headOne;
    currentTwo = headTwo;

    while ((currentOne != nullptr) && (currentTwo != nullptr))
    {
        if (currentOne->Exponent > currentTwo->Exponent)
        {
            sumCoeffcient = currentOne->Coefficient;
            addtoList(finalSum, sumCoeffcient, currentOne->Exponent);
            currentOne = currentOne->Next;
        }
        else if (currentOne->Exponent < currentTwo->Exponent)
        {
            sumCoeffcient = currentTwo->Coefficient;
            addtoList(finalSum, sumCoeffcient, currentTwo->Exponent);
            currentTwo = currentTwo->Next;
        }
        else
        {
            sumCoeffcient = currentOne->Coefficient + currentTwo->Coefficient;
            addtoList(finalSum, sumCoeffcient, currentOne->Exponent);
            currentTwo = currentTwo->Next;
            currentOne = currentOne->Next;
        }
    }

    while (currentOne != nullptr)
    {
        sumCoeffcient = currentOne->Coefficient;
        addtoList(finalSum, sumCoeffcient, currentOne->Exponent);
        currentOne = currentOne->Next;
    }

    while (currentTwo != nullptr)
    {
        sumCoeffcient = currentTwo->Coefficient;
        addtoList(finalSum, sumCoeffcient, currentTwo->Exponent);
        currentTwo = currentTwo->Next;
    }

    return finalSum;
}

/*======================================================
    Multiplies the polynomial with the term
========================================================*/

Polynomial multiplyWithTerm(Polynomial headOne, Polynomial term)
{
    Polynomial finalProduct, current;
    int productCoeffcient, productExponent;

    current = nullptr;
    finalProduct = nullptr;

    current = headOne;

    while (current != nullptr)
    {
        productCoeffcient = term->Coefficient * current->Coefficient;
        productExponent = term->Exponent + current->Exponent;

        addtoList(finalProduct, productCoeffcient, productExponent);

        current = current->Next;
    }

    return finalProduct;
}

/*======================================================
    Multiplies the polynomial with each other
========================================================*/

Polynomial multiplyPolynomial(Polynomial headOne, Polynomial headTwo)
{
    Polynomial finalProduct, inProcess, currentOne, currentTwo;

    finalProduct = nullptr;
    inProcess = nullptr;
    currentTwo = headTwo;

    while (currentTwo != nullptr)
    {
        inProcess = multiplyWithTerm(headOne, currentTwo);
        finalProduct = addition(finalProduct, inProcess);

        currentTwo = currentTwo->Next;
    }

    return finalProduct;
}

/*======================================================
    gets the number of polynomials the user Enters
========================================================*/

int getNumberOfInput(string userInput)
{
    int userInputToInt;
    while (isNumberOfInput(userInput))
    {
        cout << "Incorrect Input!!! " << endl
             << endl;
        cout << "Please Enter the number of polynomials: ";
        getline(cin, userInput);
    }

    userInputToInt = stoi(userInput);
    return userInputToInt;
}

/*======================================================
    checks if the input is correct
========================================================*/

bool isNumberOfInput(string userInput)
{
    bool isNotCorrect;
    int inputToInt;

    isNotCorrect = false;

    if (userInput.length() < 1 || userInput.length() > 2)
    {
        isNotCorrect = true;
    }

    inputToInt = ((userInput[0] - '0') * 10) + (userInput[1] - '0');

    if (inputToInt < MIN_NUMBER_OF_INPUT ||
        inputToInt > MAX_NUMBER_OF_INPUT)
    {
        isNotCorrect = true;
    }

    for (int i = 0; i < userInput.length(); i++)
    {
        if (userInput[i] < '0' || userInput[i] > '9')
        {
            isNotCorrect = true;
        }
    }

    return isNotCorrect;
}

/*======================================================
    checks if the input is has invalid characters
========================================================*/

bool differentCharFound(string userInput)
{
    bool isDiffferentChar;

    isDiffferentChar = false;

    for (int i = 0; i < userInput.length(); i++)
    {
        if ((userInput[i] < '0' || userInput[i] > '9') &&
            userInput[i] != 'x' &&
            userInput[i] != 'X' &&
            userInput[i] != '^' &&
            userInput[i] != '+' &&
            userInput[i] != '-')
        {
            isDiffferentChar = true;
        }
    }

    return isDiffferentChar;
}

int getNumberOfTerms(string polyFunction)
{
    int countt = 1;

    for (int i = 0; i < polyFunction.length(); i++)
    {
        if (polyFunction[i] == '+')
        {
            countt++;
        }
        else if (polyFunction[i] == '-' &&
                 polyFunction[i - 1] != '^' && i != 0)
        {
            countt++;
        }
    }

    return countt;
}

/*===================================================
        Checks whether the function is correct
=====================================================*/

bool checkTerms(string polyFunction)
{
    int numberOfTerms, startPos;
    bool isCoeffecientCorrect, isExponentCorrect, karatFound, correctOrder;
    string coeffecient, exponent, *coeffReturn, *termsArray;

    numberOfTerms = getNumberOfTerms(polyFunction);
    termsArray = nullptr;
    termsArray = new string[numberOfTerms];
    termsArray = getTerms(polyFunction, numberOfTerms);

    isCoeffecientCorrect = true;
    isExponentCorrect = true;
    karatFound = true;
    correctOrder = true;

    if (polyFunction[0] == '+')
    {
        isCoeffecientCorrect = false;
    }

    for (int i = 0; i < numberOfTerms; i++)
    {
        if (termsArray[i].length() == 0)
        {
            isCoeffecientCorrect = false;
        }
    }

    for (int i = 0; (i < numberOfTerms) && isCoeffecientCorrect && isExponentCorrect && karatFound; i++)
    {
        coeffReturn = getCoeffecient(termsArray[i]);
        coeffecient = coeffReturn[0];
        startPos = stoi(coeffReturn[1]);
        exponent = getExponent(termsArray[i], startPos);

        isCoeffecientCorrect = checkParsedData(coeffecient);
        isExponentCorrect = checkParsedData(exponent);

        if (isExponentCorrect)
        {
            if (stoi(exponent) > 1 || (stoi(exponent) == 1 && (startPos + 2) == termsArray[i].length()))
            {
                if (polyFunction[startPos + 1] != '^')
                {
                    karatFound = false;
                }
            }
        }
    }
    if (isCoeffecientCorrect && isExponentCorrect && karatFound)
    {
        correctOrder = checkExponentsOrder(polyFunction);
    }

    return isCoeffecientCorrect && isExponentCorrect && karatFound && correctOrder;
}

/*======================================================
    checks if the input
========================================================*/

bool checkParsedData(string dataParsed)
{
    bool dataCheck;

    dataCheck = true;

    if ((dataParsed[0] < '0' || dataParsed[0] > '9') && (dataParsed[0] != '-'))
    {
        dataCheck = false;
    }

    if (dataParsed[0] == '-' && dataParsed.length() == 1)
    {
        dataCheck = false;
    }

    for (int i = 1; i < dataParsed.length(); i++)
    {
        if (dataParsed[i] < '0' || dataParsed[i] > '9')
        {
            dataCheck = false;
        }
    }

    return dataCheck;
}

/*=======================================
    Removes anyspace from the polynomial
========================================*/

string removeSpaces(string polyTerms)
{
    string newPolyTerm;

    for (int i = 0; i < polyTerms.length(); i++)
    {
        if (polyTerms[i] != ' ')
        {
            newPolyTerm += polyTerms[i];
        }
    }
    return newPolyTerm;
}

/*============================================
    Checks if the exponent order is correct
=============================================*/

bool checkExponentsOrder(string polyFunction)
{
    int numberOfTerms, *exponentArray, startPos;
    string *termsArray, *coeffecientArray, exponent;
    bool correctOrder;

    numberOfTerms = getNumberOfTerms(polyFunction);
    exponentArray = new int[numberOfTerms];
    termsArray = getTerms(polyFunction, numberOfTerms);
    correctOrder = true;

    for (int i = 0; i < numberOfTerms; i++)
    {
        coeffecientArray = getCoeffecient(termsArray[i]);
        startPos = stoi(coeffecientArray[1]);
        exponent = getExponent(termsArray[i], startPos);
        exponentArray[i] = stoi(exponent);
    }

    for (int i = 1; i < numberOfTerms && correctOrder; i++)
    {
        if (exponentArray[i] >= exponentArray[i - 1])
        {
            correctOrder = false;
        }
    }

    return correctOrder;
}

/*=============================================
    Gets the terms and prints the terms
==============================================*/

string printTerm(Polynomial term)
{
    string finalStatement;
    if (term->Coefficient == 0)
    {
        finalStatement = "0";
    }
    else
    {
        if (term->Coefficient == 1 &&
            term->Exponent == 1)
        {
            finalStatement = "x";
        }

        if (term->Coefficient == 1 &&
            term->Exponent != 1)
        {
            finalStatement = "x^" + to_string(term->Exponent);
        }
        if (term->Coefficient != 1 &&
            term->Exponent != 1)
        {
            finalStatement = to_string(term->Coefficient) + "x^" + to_string(term->Exponent);
        }
        if (term->Exponent == 0 &&
            term->Coefficient != 0)
        {
            finalStatement = to_string(term->Coefficient);
        }
        if (term->Coefficient != 1 &&
            term->Exponent == 1)
        {
            finalStatement = to_string(term->Coefficient) + "x";
        }
        if (term->Coefficient == -1 &&
            term->Exponent == 1)
        {
            finalStatement = "-x";
        }
        if (term->Coefficient == -1 &&
            term->Exponent != 0)
        {
            finalStatement = "-x^" + to_string(term->Exponent);
        }
    }

    return finalStatement;
}

void finalPrint(Polynomial *headArray, char sign, Polynomial typeOfFunction, int numberOfPolynomial)
{
    Polynomial head;
    bool numberOfZeros = true;
    bool foundNegative = false;
    string user_str;
    string name;

    head = nullptr;

    if (sign == '+')
    {
        name = " Sum";
    }
    else
    {
        name = " Product";
    }

    cout << name << " = ";
    for (int i = 0; i < numberOfPolynomial; i++)
    {
        head = headArray[i];
        cout << "(";
        while (head)
        {
            cout << printTerm(head);
            head = head->Next;
            if (head && head->Coefficient >= 0)
            {
                cout << " + ";
            }
            else if (head && head->Coefficient > 0)
            {
                cout << " - ";
            }
        }
        cout << ")";
        if (i != numberOfPolynomial - 1)
            cout << " " << sign << " ";
    }

    cout << endl
         << setw(name.length()) << " "
         << " = ";
    head = typeOfFunction;
    while (head)
    {
        if (head->Coefficient != 0)
        {
            if (foundNegative)
            {
                cout << removeSign(user_str);
            }
            else
            {
                cout << printTerm(head);
            }

            head = head->Next;
            if (head && head->Coefficient > 0)
            {
                cout << " + ";
                foundNegative = false;
            }
            else if (head && head->Coefficient < 0)
            {
                cout << " - ";
                user_str = printTerm(head);
                foundNegative = true;
            }
        }
        else if (head->Coefficient == 0 &&
                 numberOfZeros &&
                 head == typeOfFunction &&
                 head->Next == nullptr)
        {
            cout << "0";
            head = head->Next;
            numberOfZeros = false;
        }
        else
        {
            head = head->Next;
        }
    }
    cout << endl;
}

string removeSign(string userCoeff)
{
    string newInput = "";

    for (int i = 1; i < userCoeff.length(); i++)
    {
        newInput += userCoeff[i];
    }

    return newInput;
}