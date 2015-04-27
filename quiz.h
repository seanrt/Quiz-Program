using namespace std;

struct {string *q; string *a; string cat;} typedef mc;
typedef std::string string;

ifstream fin;

int* makearr(int size) { // Allocate memory for an int array of a certain size
    int *p=new int[size];

    for (int i=0;i<size;i++) { // All elements are initialized to 0
        p[i]=0;
    }
    return p;
}

void initializetags(int q[], int a[], int size) { // Set the tags to integers from 1 to size
    for (int n=0;n<size;n++) {
        q [n]=n+1;
        a [n]=n+1;
    }
}

void resettags(int q[], int a[]) { // This function resets the questions and answers
    delete[] q;
    delete[] a;
}

void printqas(string *q, string *a, int size) { // Output the questions and answers
    for (int i=0;i<size;i++) { 
         cout << std::setw(6) << *(q+i) 
              << std::setw(6) << *(a+i) << endl;
    }
}

void printcats(string *c, int size) {
    for (int i=0;i<size;i++) {
        cout << std::setw(6) << *(c+i);
    }
    cout << endl;
}

void chooseCat(int &choice, string* cats, int size) { // Take in the category choice, loop until a valid choice is given
    string input;

    cout << "Select which category of words to be tested on" << endl << endl;    
    for (int i=0;i<size;i++) {
        cout << i+1 << ") " << *(cats+i) << endl;
    }
    cout << "0) Exit the program" << endl << endl;

    for (;;) {          // Input the category choice
        for (;;) {
            cout << "Category: ";
            cin >> input;
            std::stringstream ss(input);
            if(ss >> choice) {
                break;
            }
        }
        if ((choice>=0) && (choice<=size)) {
            break;
        } 
    }
}

void chooseInput(int &choice) { // This function chooses between the French and Russian tests
    string input;

    cout << "Select which language to be tested on" << endl << endl    
         << "1) Russian" << endl
         << "2) French" << endl
         << "0) Exit the program" << endl << endl;

    for (;;) {          // Input the category choice
        for (;;) {
            cout << "Language: ";
            cin >> input;
            std::stringstream ss(input);
            if(ss >> choice) {
                break;
            }
        }
        if ((choice>=0) && (choice<=2)) {
            break;
        } 
    }
}

void shuffle(int questions[], int answers[], int size) { // Shuffle the question and answer elements
    int n, x, *q=makearr(size), *a=makearr(size), *flag=makearr(size);

    for (n=0;n<size;n++){ // Shuffle the elements
        for (;;) {
            x=rand()%size; // x is a random number from 0 to size-1
            if (flag[x]==0) {		// if the element has not been shuffled yet
                q[x]=questions[n];
                a[x]=answers[n];
                flag[x]=1;
                break;
            }
        }
    }

    for (n=0;n<size;n++) { // Assign the shuffled elements
        questions[n]=q[n];
        answers[n]=a[n];	
    }

    delete[] q;
    delete[] a;
    delete[] flag;
}

void askqs(string *qq, string *aa, int *q, int *a, int answer, int size, int & count, int & cont) {
    int n, choice;
    string input;

    cout << *(qq+q[0]-1) << endl; // Output the question and the answers
    cout << "1) " << *(aa+a[0]-1) << endl
         << "2) " << *(aa+a[1]-1) << endl
         << "3) " << *(aa+a[2]-1) << endl
         << "4) " << *(aa+a[3]-1) << endl << endl;

    for (;;) { // Take in their answer
        for (;;) {
            cout << "Your answer is (enter 0 to exit): ";
            cin >> input;
            std::stringstream ss(input);
            if(ss >> choice) {
                break;
            }
        }
        if ((choice>=0) && (choice<=4)) {
            break;
        }
    }	 	 

    if (choice==0) { // If the answer was zero, we exit
        cont=0;
        return;
    }

    if ((choice-1)==answer) { // If the answer is correct, increment the count
        cout << "Correct." << endl << endl;
        count++;
    } else {
        cout << "Incorrect. The answer was '" << *(aa+a[answer]-1) << "'." << endl << endl;
    }
}

void assignask(string *qq, string *aa, int qs[], int as[], int &n, int & c, int size, int & count, int & cont) {
    int x, *a=makearr(size), *q=makearr(size), *flag=makearr(size);
    string *temp=new string[size];

    c=rand()%4; 
    q[0]=qs[n]; // Assign the first question and answer
    a[c]=as[n];
    flag[n]=1;

    for (int i=0;i<4;i++) { // Assign the other three questions and answers
        if (i!=c) { // The first answer was already assigned
            for (;;) {
                x=rand()%size; 

                if (flag[x]==0) {
                    a[i]=as[x];
                    flag[x]=1;	
                    break;
                }
            }
        }
    }	
    
    if (rand()%2==1) { // Half the time we swap the questions and answers
        temp=aa;
        aa=qq;
        qq=temp;
    }

    askqs(qq, aa, q, a, c, size, count, cont);
    
    if (cont==0) { // If we are not continuing n must be decrimented
        n--;
    }
    
    temp=NULL;
    delete[] temp;
    delete[] a;
    delete[] q;
    delete[] flag;
}

void question(string *qq, string *aa, string cat, int q[], int a[], int size, int &qcount, int &ccount) {
    int n, x, c, count=0, cont=1;
    int *flag=makearr(size);

    cout << cat << endl << endl;

    for (n=0 ; (n<size) && (cont==1) ; n++) { // Every question is asked as long as the user wants to continue
        for (;;) {
            x=rand()%size; 

            if (flag[x]==0) { // If the question has not been asked yet
                assignask(qq, aa, q, a, n, c, size, count, cont);
                flag[x]=1;
                break;
            }
        }		
    }

    if (n>0) { // Only output the mark if a question was answered
        cout << "You answered " << count << " out of " << n << " questions (" 
             << std::fixed << std::setprecision(2) 
             << count*100.0/n << "%) correctly." << endl;
    }
    cout << endl;

    qcount+=n;
    ccount+=count;

    delete[] flag;
}

mc filein(int *size,int category,int lang) { // This function assigns the questions and answers from strings in the input file
    mc temp;
    int i,j;

    if (lang==1) {
        fin.open("russianinput");
    }
    else {
        fin.open("frenchinput");
    }
 
    for (j=1;j<category;j++) {          // This is skipped if category is 1
        fin >> *size;                   // The size is taken in
        fin >> temp.cat;                // This skips the category
        getline(fin,temp.cat,'%');      // The description is taken in

        for (i=0;i<((*size)*2);i++) { // All the data for that category is skipped
            fin >> temp.cat;
        }
    }
    
    fin >> *size;                 // This takes in the number of questions and answers  
    fin >> temp.cat;              // This skips the category
    getline(fin,temp.cat,'%');    // This takes in the description

    temp.q=new string[*size];
    temp.a=new string[*size];

    for (i=0;i<*size;i++) { // This loop takes in questions        
        fin >> *(temp.q+i);
        fin >> *(temp.a+i);
    }
    fin.close();  

    return temp;
}

string* catsin(int &size, int lang) {
    string *temp,temp2,input;
    int i,j,k;
    
    if (lang==1) {
        fin.open("russianinput");
    }
    else {
        fin.open("frenchinput");
    }
    while( !( fin.eof() ) ) { // This loop finds the number of categories         
        fin >> j;                  
        fin >> temp2;     
        getline(fin,temp2,'%');     
        
        for (i=0;i<((j)*2);i++) { 
            fin >> temp2;
        }
        
        if ( !( fin.eof() ) ) {
            size++;
        }
    } 
    fin.close();
    
    temp=new string[size]; // The memory for the array of categories is allocated

    if (lang==1) {
        fin.open("russianinput");
    }
    else {
        fin.open("frenchinput");
    }
    for (i=0;i<size;i++) {  // This loop takes in the categories
        fin >> j;                  
        fin >> *(temp+i);     
        getline(fin,temp2,'%');     
        
        for (k=0;k<((j)*2);k++) { 
            fin >> temp2;
        }   
    }
    fin.close();

    return temp;
}
