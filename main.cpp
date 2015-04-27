// Russian vocabulary quiz

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <time.h>
#include <cstdlib>
#include "quiz.h"

using namespace std;

int main (void) {

    srand(time(NULL));
    
    int cat, lang, catsize=0, qcount=0, ccount=0, size, *tagq, *taga;
    mc mcs;    

    chooseInput(lang); // The language of study is chosen
    cout << endl;
    if (lang==0) 
        return 0; 

    string *cats=catsin(catsize,lang); // The categories are assigned
   
    for (;;) {
        chooseCat(cat,cats,catsize); // Take in the category and leave the loop if desired
        if (cat==0)
            break;

        mcs=filein(&size,cat,lang); // Assign the questions, answers, and size
        tagq=makearr(size);
        taga=makearr(size);
        initializetags(tagq,taga,size);

        shuffle(tagq,taga,size); // Shuffle the tags
        question(mcs.q,mcs.a,mcs.cat,tagq,taga,size,qcount,ccount); // Ask the questions for the chosen category
        
        resettags(tagq,taga); // Reset the tags, questions, and answers
        delete [] mcs.q;
        delete [] mcs.a;
    }
    
    if (qcount>0) { // Output the stats if questions were answered
        cout << endl 
             << "Results" << endl
             << "Correct answers: " << ccount << endl
             << "Incorrect answers: " << qcount-ccount << endl
             << "Questions answered: " << qcount << endl
             << std::fixed << std::setprecision(2) 
             << "Score: " << ccount*100.0/qcount << "%" << endl << endl;
    }

    return 0;
}

