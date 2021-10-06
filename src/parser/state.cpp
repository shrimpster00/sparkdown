// src/parser/state.cpp
// v. 0.4.1
//
// Author: Cayden Lund
//   Date: 10/05/2021
//
// This file is part of mark-sideways, a new markup/markdown language
// for quickly writing and formatting notes.
//
// This file contains the implementation of the State class.
//
// Copyright (C) 2021 Cayden Lund <https://github.com/shrimpster00>
// License: MIT (https://opensource.org/licenses/MIT)

#include <string>
#include <regex>
#include <stack>

// The header file for the State class.
#include "state.hpp"

// The mark_sideways namespace contains all the classes and methods of the mark-sideways library.
namespace mark_sideways
{
    // Constructor.
    State::State()
    {
        indentation = 0;
        verbatim = false;
        preline = "";
        line = "";
    }

    // Returns a string of spaces to indent the next line.
    std::string State::indent()
    {
        std::string result;
        for (int i = 0; i < indentation; i++)
        {
            result += "    ";
        }
        return result;
    }

    // Returns true if verbatim mode is on.
    bool State::is_verbatim()
    {
        return verbatim;
    }

    // Returns the necessary \begin{verbatim} or \end{verbatim} commands.
    std::string State::toggle_verbatim()
    {
        if (verbatim)
        {
            verbatim = false;
            return "\\end{verbatim}";
        }
        else
        {
            verbatim = true;
            return "\\begin{verbatim}";
        }
    }

    // Sets the indentation to the given value.
    void State::set_indentation(int indentation)
    {
        while (this->indentation > indentation)
        {
            if (levels.top() == itemize)
            {
                decrease_itemize();
            }
            else
            {
                decrease_enumerate();
            }
        }
    }

    // Sets the idemization level to itemize.
    //
    // * int level - The new idemization level.
    void State::begin_itemize(int level)
    {
        while ((int)levels.size() > level)
        {
            if (levels.top() == itemize)
            {
                decrease_itemize();
            }
            else
            {
                decrease_enumerate();
            }
        }
        while ((int)levels.size() < level)
        {
            increase_itemize();
        }
        if (level > 0 && levels.top() != itemize)
        {
            decrease_enumerate();
            increase_itemize();
        }
    }

    // Sets the enumeration level to enumerate.
    //
    // * int level - The new enumeration level.
    void State::begin_enumerate(int level)
    {
        while ((int)levels.size() > level)
        {
            if (levels.top() == itemize)
            {
                decrease_itemize();
            }
            else
            {
                decrease_enumerate();
            }
        }
        while ((int)levels.size() < level)
        {
            increase_enumerate();
        }
        if (level > 0 && levels.top() != enumerate)
        {
            decrease_itemize();
            increase_enumerate();
        }
    }

    // Returns the current line.
    std::string State::get_line()
    {
        return line;
    }

    // Sets the current line.
    //
    // * std::string line - The new current line.
    void State::set_line(std::string line)
    {
        this->line = line;
    }

    // Returns the final product.
    std::string State::get_product()
    {
        std::string indentation_string = "";
        if (!verbatim)
        {
            indentation_string = indent();
        }
        std::string product = preline + indentation_string + line;
        preline = "";
        line = "";
        return product;
    }

    // Increases the itemize level.
    void State::increase_itemize()
    {
        preline += indent() + "\\begin{itemize}\n";
        levels.push(itemize);
        indentation++;
    }

    // Decreases the itemize level.
    void State::decrease_itemize()
    {
        indentation--;
        preline += indent() + "\\end{itemize}\n";
        if (levels.top() != itemize)
        {
            throw std::runtime_error("Itemize level mismatch.");
            exit(1);
        }
        levels.pop();
    }

    // Increases the enumerate level.
    void State::increase_enumerate()
    {
        preline += indent() + "\\begin{enumerate}\n";
        levels.push(enumerate);
        indentation++;
    }

    // Decreases the enumerate level.
    void State::decrease_enumerate()
    {
        indentation--;
        preline += indent() + "\\end{enumerate}\n";
        if (levels.top() != enumerate)
        {
            throw std::runtime_error("Enumerate level mismatch.");
            exit(1);
        }
        levels.pop();
    }
}
