#include<bits/stdc++.h>
using namespace std;

vector<int> state, best_state_H, best_state_S;
#define VECTOR vector<int>

int conflict_HC, conflict_SA;
int iteration_HC, iteration_SA;

int count_conflicts(VECTOR state){
    int N= state.size();


    int conflict=0;

    for(int i=0; i<N ; i++){

        for(int j=i+1; j<N; j++)
        {
            int slope=fabs( 10*((state[i]- state[j])*1.0/(i-j)*1.0));
          //  cout<<slope<<endl;

            if( slope==10 || slope==0 )
                {
            //        cout<<i<<","<<state[i]<<"  "<<j<<","<<state[j]<<endl;
                    conflict++;
                }

        }

    }
    return conflict;

}


void print_vector(VECTOR state){
    int N= state.size();
    for(int i=0;i <N; i++)
        cout<<state[i]<<" ";
    cout<<endl;


}

void hill_climbing(VECTOR initial_state){

    int max_step=0;
    VECTOR current= initial_state;
    vector< vector<int > > neighbours ;
    int NC=0;
    int N= current.size();
    int minimum_conflict=count_conflicts(current);
    int iteration=0;
    cout<<"Initial state"<<endl;
    print_vector(current);
    cout<<"Initial conflict: "<<minimum_conflict<<endl;
    cout<<"---------------------------------------"<<endl;


    while(true){

        int index=0;
        int index_next=0;

        int flag= true;

        for(int i=0; i<N; i++){

            for(int j=0; j<N; j++){

                if(current[i]!=j){

                        VECTOR temp= current;
                        temp[i]=j;
                        int conflict=count_conflicts(temp);
                        cout<<conflict<<endl;
                        if(conflict < minimum_conflict){
                                index_next=index;
                                minimum_conflict= conflict;
                                flag=false;

                        }

                        neighbours.push_back(temp);
                        index++;

                }

            }

        }


        if(flag)
        {

            print_vector(current);
            cout<<"Current conflict: :"<<minimum_conflict<<endl;
            cout<<"Number of iterations :"<<iteration<<endl;
            cout<<"---------------------------------------"<<endl;
            break;
        }

        current= neighbours[index_next];
        neighbours.clear();
        iteration++;
    }
    return;

}



void simulated_annealing(VECTOR state){

    int conflict=0;
    VECTOR best_state;
    int minimum_conflict= count_conflicts(state);


    VECTOR current=state;
    vector< VECTOR > neighbours;
    int T_initial, u_constant;
    int T;
    int t=0;
    int N= state.size();
    int current_conflict= count_conflicts(state);

    T_initial=100;
    u_constant=1;

    cout<<"Initial state:"<<endl;
    print_vector(current);
    cout<<"Initial conflict: "<<current_conflict<<endl;
    cout<<"---------------------------------------"<<endl;



    while(true){
            T= T_initial - u_constant * t;


            if(T==0){
                    cout<<"Final state and conflict count."<<endl;
                    print_vector(current);
                    cout<<"Number of conflict: "<<count_conflicts(current)<<endl;

                    cout<<"Best state and best minimum conflict count so far."<<endl;
                    print_vector(best_state);
                    cout<<"Best conflict count: "<<minimum_conflict<<endl;

                    cout<<"---------------------------------------"<<endl;
                    break;

            }



            int index=0;

            int flag= true;

            for(int i=0; i<N; i++){

                for(int j=0; j<N; j++){

                    if(current[i]!=j){

                            VECTOR temp= current;
                            temp[i]=j;
                            neighbours.push_back(temp);
                            index++;

                    }

                }

            }


            int index_neighbour= rand() % index;

            VECTOR temp= neighbours[index_neighbour];
            int conflict= count_conflicts(temp);
            int deltaE= current_conflict - conflict; // this is the favorable situation, we want to minimize conflict

            if(conflict< minimum_conflict )
            {
                    minimum_conflict=conflict;
                    best_state= temp;
            }

            if(deltaE > 0){

                current=temp;
                current_conflict=conflict;


            }

            else if(deltaE < 0 ){

                double probability = exp(deltaE*1.0 / T*1.0);

                double random_number= (double) rand()*1.0/RAND_MAX*1.0;

                if(random_number< probability)
                   {

                       cout<<"RN: "<<random_number<<" "<<"Prob: "<<probability<<endl;
                       current=temp;
                       current_conflict=conflict;

                   }


            }


            neighbours.clear();

            t++;



    }


    return;

}

void generate_random_state(VECTOR& state){

    int N= state.size();

    for(int i=0; i<N; i++)
        state[i]=rand() % N;
}

int main(void){

    srand(time(NULL));

    int N;
    cin>>N;

    VECTOR state;
    state.resize(N);

    for(int i=0; i<N; i++)
            cin>>state[i];
    simulated_annealing(state);

//    hill_climbing(state);

    //state.resize(N);


 //   for(int i=0; i<10; i++){
   //     generate_random_state(state);
     //   hill_climbing(state);
     //   simulated_annealing(state);
   // }

    //for(int i=0; i<N;i++)
       // cout<<best_state_H[i]<<" ";
    //cout<<endl;


    //for(int i=0; i<N;i++)
      //  cout<<best_state_S[i]<<" ";
    //cout<<endl;



    return 0;

}
