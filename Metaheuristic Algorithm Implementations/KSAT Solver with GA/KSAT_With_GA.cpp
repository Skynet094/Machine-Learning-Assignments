#include<bits/stdc++.h>
using namespace std;
#define dll long long int

#define VECTOR vector<int>
#define BOOL_MAP map<int,bool>

int fitness(BOOL_MAP single, int m);


VECTOR individual, best;
vector< VECTOR > expression;


vector< BOOL_MAP > Population, Next_gen_offspring;
typedef pair<BOOL_MAP, BOOL_MAP> OFFSPRINGS;

BOOL_MAP var_list;


map<int , int> hash_val;


//unique hash value for unique individual
int hash_function(VECTOR test){

    dll value=0;

    // expecting a string of 0 and 1 here, will generate a unique binary number

    for(int i=0; i< test.size(); i++){
        value+=test[i] * pow(2, i);

    }
    return value;

}







void Mutate(BOOL_MAP& child){

    int mutation_rate= 5; //1 percent

    map<int, bool> :: iterator iter;

    iter= child.begin();
    int location=0;


    while(iter != child.end()){

        int flip_probability=  rand()  % 101;
      //  cout<<"flip probability for mutation: "<<flip_probability<<endl;


        if(flip_probability < mutation_rate){
          cout<<"Mutation happened at bit_location= "<<location<<endl;
        //flipping the bits
            if(iter->second ==1)
                iter->second=0;
            else
                iter->second=1;

        }

        iter++;
        location++;

    }


}



BOOL_MAP tournament_selection(int m=0){

        int t=5;

        if(Population.size() < t)
            t= Population.size();

        vector<BOOL_MAP> selected_parents;
        BOOL_MAP best;

        for(int i=0; i< t; i++)
            selected_parents.push_back(Population[rand() % Population.size()]);

        int best_fitness=-1;

        for(int i=0; i<selected_parents.size(); i++)
        {

            int fit= fitness(selected_parents[i],m );


            if( fit > best_fitness){
                    best_fitness=fit;
                    best= selected_parents[i];

            }
        }

        return best;

}


void print_individual(BOOL_MAP solution){

      //  cout<<"Solution is: "<<endl;
        BOOL_MAP::iterator iter;
        iter=solution.begin();

        while(iter!= solution.end()){
            cout<<bool(iter->second)<<" ";
            //cout<<iter->second;
            iter++;
        }

    cout<<endl;
        //cout<<endl<<"Fitness value for this solution is: "<<fitness(solution)<<endl;

}



OFFSPRINGS crossover(BOOL_MAP parentA, BOOL_MAP parentB){

    //cout<<"----------------------------Crossover: --------------------------"<<endl;
//    cout<<"Parent: ";
    //print_individual(parentA);
    //print_individual(parentB);

    OFFSPRINGS children;

    int chance= rand() % 2;

    int len= individual.size();

    if(chance==0){
        // ONE POINT CROSSOVER

        int c= rand() % len;
        cout<<"One point crossover, c= "<<c<<endl;
        int counter=0;

        map<int, bool>:: iterator iterA, iterB;
        iterA=  parentA.begin();
        iterB= parentB.begin();


        while(iterA != parentA.end()){

            if(counter>=c){
                // swapping bits
                int temp= iterA->second;
                iterA->second= iterB->second;
                iterB->second= temp;
            }

            iterA++;
            iterB++;
            counter++;
        }



    }

    else if(chance == 1){


        int c= rand() % len;
        int d= rand() % len;
        if(c > d)
            swap(c,d); // so, c < d

        // TWO POINT CROSSOVER
        cout<<"Two point crossover, c= "<<c<<" d= "<<d<<endl;

        int counter=0;

        map<int, bool>:: iterator iterA, iterB;
        iterA=  parentA.begin();
        iterB= parentB.begin();


        while(iterA != parentA.end()){

            if(counter>=c && counter<=d){
                // swapping bits
                int temp= iterA->second;
                iterA->second= iterB->second;
                iterB->second= temp;
            }

            iterA++;
            iterB++;
            counter++;
        }





    }


    children.first= parentA;
    children.second= parentB;
    //print_individual(children.first);
    //print_individual(children.second);

  //  cout<<"------------------------Crossover ends:---------------------------"<<endl;
    return children;

}


// m ==0 means K-SAT, m>=1 means that, Exactly-m K-SAT

BOOL_MAP genetic_algorithm( int pop_size, int m=0){

    //

    int unique_count=0;


    while(unique_count!= pop_size){

        VECTOR temp;
        temp.resize(individual.size());
        int len= temp.size();

        for(int i=0; i<len; i++){
            temp[i]= rand() % 2;
        }

        int value= hash_function(temp);



        if(hash_val.find(value) !=  hash_val.end()) // this entry was included before
                {
             //       cout<<"NON UNIQUE."<<endl;
                    continue;
                }
        else{

                //include this entry to the hash_val map
            hash_val[value]=-1;
            BOOL_MAP temp_map;

            for(int i=0; i<len; i++){
                temp_map[individual[i]]=temp[i];

            }

            Population.push_back(temp_map); //adding in the population
            int fit_value;
            fit_value=fitness(temp_map, m);

             unique_count++;

        }


    }  // we have unique individuals at this stage.



    // here starts the game!

    int max_generations=5000* 2;
    int iter_gen=0;
    int best_fitness=-1;
    BOOL_MAP best;
    int max_clauses=expression.size();



    while(iter_gen < max_generations){

        // find the best individual

        for(int i=0; i<pop_size; i++){

            int fit_value;

            fit_value=fitness(Population[i], m);


            if(fit_value > best_fitness)
            {
                best_fitness=fit_value;
                best= Population[i];
            }
        }



        cout<<"------------------------------------GENERATION NUMBER: "<<iter_gen<<" Best fitness: "<<fitness(best, m)<<"------------------------------------------"<<endl;


    //check if it's the global optima, if so, break the loop and run away!
        Next_gen_offspring.clear();

        if(best_fitness == max_clauses)
            break;

        //do the selection and crossover, mutation, whatever!
        for(int i=0; i< pop_size/2; i++){

               //    cout<<"Trying to copulate. "<<i<<endl;

                BOOL_MAP parentA = tournament_selection();
                BOOL_MAP parentB = tournament_selection();


               // cout<<"Selection done."<<endl;
                cout<<"----------------------------------------Crossover-----------------------------------------------"<<endl;
                print_individual(parentA);
                print_individual(parentB);
                OFFSPRINGS children = crossover(parentA, parentB);

               // cout<<"Crossover done."<<endl;
                print_individual(children.first);
                print_individual(children.second);

                cout<<"----------------------------------------Crossover done-----------------------------------------------"<<endl;

                cout<<"-----------------------------------------Mutation-------------------------------------------------------"<<endl;

                Mutate(children.first);
                Mutate(children.second);
                print_individual(children.first);
                print_individual(children.second);

                cout<<"-----------------------------------------Mutation done-----------------------------------------------------"<<endl;

               // cout<<"Mutation done."<<endl;

                Next_gen_offspring.push_back(children.first);
                Next_gen_offspring.push_back(children.second);


        }

        Population= Next_gen_offspring;
        iter_gen++;
    }


    return best;

    //cout<<"unique_count= "<<unique_count<<endl;

}















int fitness(BOOL_MAP single, int m=0){


    int fit_value=0;

    int no_clauses= expression.size();

    int no_literals= expression[0].size();


    if(m==0){
            for(int i=0; i< no_clauses; i++){

                for(int j=0; j< no_literals; j++){
                    int current_value= expression[i][j];

                    if(current_value<0){

                            int assignment= single[abs(current_value)];
                            if(assignment==0) //it makes the literal true
                            {
                                fit_value++;
                                break;
                            }
                    }

                    else if(current_value>0){

                      int assignment= single[abs(current_value)];
                            if(assignment==1) //it makes the literal true
                            {
                                fit_value++;
                                break;
                            }

                    }

                }

            }
    }

    else if(m>0){


            for(int i=0; i< no_clauses; i++){

                int max_true=0;


                for(int j=0; j< no_literals; j++){
                    int current_value= expression[i][j];

                    if(current_value<0){

                            int assignment= single[abs(current_value)];
                            if(assignment==0) //it makes the literal true
                            {
                                max_true++;

                            }
                    }

                    else if(current_value>0){

                      int assignment= single[abs(current_value)];
                            if(assignment==1) //it makes the literal true
                            {
                                max_true++;
                            }

                    }

                }

                if(max_true==m)
                    fit_value++;

            }




    }

    return fit_value;


}



void print_population(int m=0){

    cout<<"Printing all population members"<<"size= "<<Population.size()<<endl;

    int len= Population.size();

    for(int i=0; i< len;  i++){
        BOOL_MAP::iterator iter;
        iter=Population[i].begin();

        while(iter!=Population[i].end()){
            cout<<"x"<<iter->first<<"= "<<bool(iter->second)<<" ";
            //cout<<iter->second;
            iter++;
        }

        cout<<endl<<"Fitness value for this thing is: "<<fitness(Population[i],m )<<endl;


    }


}


void print_solution(BOOL_MAP solution, int m=0){

        cout<<"Solution is: "<<endl;
        BOOL_MAP::iterator iter;
        iter=solution.begin();

        while(iter!= solution.end()){
            cout<<"x"<<iter->first<<"= "<<bool(iter->second)<<" ";
            //cout<<iter->second;
            iter++;
        }


        cout<<endl<<"Fitness value for this solution is: "<<fitness(solution, m)<<endl;



}

void print_expression(){

    cout<<"Given expression: "<<endl;

    for(int i=0; i< expression.size(); i++){

        cout<<"(";

        for(int j=0; j< expression[0].size(); j++){


            if(expression[i][j]>0)
                cout<<"x"<<expression[i][j];
            else if(expression[i][j]<0)
                cout<<"~x"<<expression[i][j] * -1;
            if(j != expression[0].size() -1)
                cout<<" V ";
        }


        cout<<")";
    if(expression.size() -1 != i)
        cout<<" A ";
    }


    cout<<endl;


}

/*
Demo expression:

10 2
-9 3 -4 -2 -6 -7 -1 -10 -8 -5
-5 -6 1 3 9 -2 7 -8 -10 -4


5 5
2 -5 -4 -3 1
-2 -3 -5 1 -4
5 -4 -2 1 -3
2 3 5 1 4
-3 1 5 -4 2


3 10
-13 16 7
16 -17 8
8 3 4
6 -17 10
-12 20 -2
-18 9 14
12 -19 15
1 4 8
-3 -20 8
-14 -16 -19


*/


int main(void){

    srand (time(NULL));


    int N_clause, N_literals;

    cin>>N_literals>>N_clause;


    expression.resize(N_clause);

    //store expression and extract variables

    for(int i=0; i<N_clause; i++){

        for(int j=0; j< N_literals; j++){
            int temp;
            cin>>temp;

            var_list[abs(temp)]=0; //mapping, for future lookup!
            expression[i].push_back(temp);

        }

    }


    cout<<"Input done."<<endl;

    map<int, bool>::iterator iter;

    iter= var_list.begin();

    while(iter != var_list.end() ){
        individual.push_back((*iter).first);
       // cout<< (*iter).first <<endl;
        iter++;

    }


   // int pop_size= ceil(pow(2, individual.size()) *1.0 * 0.001);

   int pop_size=16;
   int m=0;

    cout<<"Initial population size: "<<pop_size<<endl;

    BOOL_MAP best= genetic_algorithm(pop_size,m);
   // cout<<"HOLA AMIGO"<<endl;
    //print_population();

    print_expression();
    print_solution(best, m);

    return 0;
}
