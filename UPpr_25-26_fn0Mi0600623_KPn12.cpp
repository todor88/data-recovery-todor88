// кп1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/**
*
* Solution to course project # 12
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2025/2026
*
* @author Todor Kirilov
* @idnumber 0MI0600623
* @compiler <използван компилатор - VC
*
* <предназначение на файла>
*
*/





#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

const int MAX = 1024;
const char* FILE_NAME = "ataka.txt"; 


int my_strlen(const char* str )
{
    int l = 0;
    while (str[l] != '\0') l++;
    return l;
}

void my_strcopy(char* dest, const char* src)
{
    int i = 0;
    while (src[i] != '\0') { dest[i] = src[i]; i++; 
    }
    dest[i] = '\0';
}

bool isLetter(char c) 
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

char flip(char c, int bit) 
{
    return c ^ (1 << bit);
}


bool load_file(char* buffer) 
{
    std::ifstream f(FILE_NAME);
    if (!f) return false;
    f.getline(buffer, MAX);
    f.close();
    return (my_strlen(buffer) > 0);
}


void damaged_text(char* curr, const char* org, bool* m, double rate) 
{
    int n = my_strlen(org);
    int letter_indices[MAX], count = 0;
    for (int i = 0; i < n; i++) {
        if (isLetter(org[i])) letter_indices[count++] = i;
    }

    int to_damage = (int)(count * rate);
    for (int i = 0; i < to_damage; i++)
    {
        int idx = letter_indices[std::rand() % count];
        if (!m[idx]) 
        {
            m[idx] = true;
            curr[idx] = flip(org[idx], std::rand() % 6);
        }
        else i--;
    }
}


void draw(const char* curr, const char* org, const bool* m)
{
    std::cout << "\033[2J\033[H--- CONSOLE GAME ---\n\n";
    for (int i = 0; curr[i] != '\0'; i++)
    {
        if (m[i]) 
        {
            if (curr[i] == org[i]) std::cout << "\033[32m" << curr[i] << "\033[0m";
            else std::cout << "\033[31m" << curr[i] << "\033[0m";
        }
        else std::cout << curr[i];
    }
    std::cout << "\n\n---------------------\n";
}


int Find_Word(const char* t, int n) 
{
    int count = 0;
    bool in_word = false;
    for (int i = 0; t[i] != '\0'; i++) 
    {
        if (t[i] != ' ' && !in_word)
        {
            count++;
            in_word = true;
            if (count == n) return i;
        }
        else if (t[i] == ' ') in_word = false;
    }
    return -1;
}


int main() {
    std::srand(std::time(0));
    char org[MAX] = "", curr[MAX] = "";
    bool mask[MAX] = { false };
    int mistakes = 0;

    if (!load_file(org))
    {
        std::cout << "ERROR: Cannot read '" << FILE_NAME << "' or file is empty!\n";
        return 1;
    }

    my_strcopy(curr, org);
    damaged_text(curr, org, mask, 0.4);

    while (true) 
    {
        draw(curr, org, mask);

        bool win = true;
        for (int i = 0; org[i] != '\0'; i++) 
        {
            if (curr[i] != org[i])
            {
                win = false;
            }
        }
            if (win)
            {
                std::cout << "VICTORY! Mistakes: " << mistakes << "\n";
                break;
            }

            int w_num;
            std::cout << "Word number (1, 2...): ";
            if (!(std::cin >> w_num))
            {
                break;
            }

            int start = Find_Word(curr, w_num);
            if (start == -1)
            {
                continue;
            }

            int pos;
            std::cout << "Character position in word: ";
            std::cin >> pos;

            int target = start + pos - 1;
            if (target < 0 || target >= my_strlen(org) || org[target] == ' ')
            {
                continue;
            }

            std::cout << "\nOptions for '" << curr[target] << "':\n0) Cancel\n";
            for (int i = 0; i < 6; i++)
            {
                std::cout << i + 1 << ") " << flip(curr[target], i) << "\n";
            }

            int choice;
            std::cout << "Your choice: ";
            std::cin >> choice;

            if (choice >= 1 && choice <= 6)
            {
                char next = flip(curr[target], choice - 1);
                if (next != org[target])
                    mistakes++;
                curr[target] = next;
            }
        
    }
    return 0;
}