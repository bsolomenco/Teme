#pragma once

typedef int Info;

//Nod structure used in list based stack or queue
struct Nod{
    Info    info = {};
    Nod*    next = nullptr;
};