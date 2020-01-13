#include<bits/stdc++.h>
using namespace std;

vector<vector<int> >  csvFileDataMatrix;  /// csv file data will be laid out as a 2D matrix , where each row correspond to each data instance

vector<vector<int> > TrainingDataMatrix1, TrainingDataMatrix2,TrainingDataMatrix3, TrainingDataMatrix4, TrainingDataMatrix5;

vector<vector<int> > TestDataMatrix1, TestDataMatrix2, TestDataMatrix3, TestDataMatrix4, TestDataMatrix5;


vector<vector<int> > uniqueAttributePerFeatureMatrix1,uniqueAttributePerFeatureMatrix2,uniqueAttributePerFeatureMatrix3,uniqueAttributePerFeatureMatrix4,uniqueAttributePerFeatureMatrix5 ; /// per feature te koyta kore attribute hoy

vector<string> column_names;

int majority_label=-1;


void PrintDataMatrix(vector<vector<int> > DataMatrix);



/// defining information per node

struct TreeNode{

       int split_feature;
       vector<TreeNode*> child_nodes;
       vector<int> child_attributes;  /// child_attribute[i] will be associated with child_nodes[i]
       bool isPureNode;
       int nodeLabel; /// node label if it's the leaf

};

vector<vector<int> > splitDataMatrix(vector<vector<int> > DataMatrix, int splitFeature, int splitValue){

    vector<vector<int> > newDataMatrix;

    // two things
    // one: do not include the column in the new DataMatrix
    //second: do not take the data instances where split_feature_value= splitValue
    int SIZE_row= DataMatrix.size(); // number of data instance
    int SIZE_col = DataMatrix[0].size(); // number of columns

    // jokhon ekta ekta kore column delete korbo, tokhon to , column shift hoye ek ghor bame ashbe,  FeatureMatrix er sathe mismatch hoye jabe , tai
    // but first row te feature er information thake, oi ta use kore determine korbo je kon index ta kon Feature er , ba Feature ta kon column e ase
    int featureIndex;

    for(int i=0; i< DataMatrix[0].size(); i++)
    {
        if(DataMatrix[0][i] == splitFeature){
            featureIndex = i;
            break;
        }
    }
  //  cout<<"Feature index: "<<featureIndex<<endl;
   // cout<<"Size ROW: "<< SIZE_row<<" "<<"SIZE COL: " <<SIZE_col<<endl;


    vector<int> TEMP;

    for(int i=0; i< SIZE_col ; i++)
    {
        if(i != featureIndex)
            TEMP.push_back(DataMatrix[0][i]);

    }

    newDataMatrix.push_back(TEMP);


    for(int i=1; i< SIZE_row; i++){

        vector<int> temp;
        // skip row if splitValue== featureValue
        if(DataMatrix[i][featureIndex] != splitValue)
            continue;
       // cout<<"YES"<<endl;

        for(int j=0; j<SIZE_col; j++){
            if(j != featureIndex) // skip the feature column
                temp.push_back(DataMatrix[i][j]);

        }
       // cout<<"YES?"<<endl;
        newDataMatrix.push_back(temp);
    }


    return newDataMatrix;
}


int getSplitFeature(vector<vector<int> > DataMatrix, vector<vector<int> > FeatureMatrix){


    ///calculate information gain


    // IG(S, Sx)= H(S) - (|S1|/|S|)* H(S1) -(|S2|/|S|)* H(S2) -(|S3|/|S|)* H(S3)....-(|Sn|/|S|)* H(Sn)
    // calculate entropy for the root node,  save the value of |S| , H(S)
    // choose each feature
    // for each feature, for each value of each feature , calculate entropy, save the value of |Sx|


    double S_root_pos;
    double S_root_neg;
    double H_S;
    map<int, int> count_class_label;
    double MAX_INFORMATION_GAIN= - INT_MAX  * 1.0;
    int class_label_A= 1; //picking one label, malignant

    for(int i=1; i<DataMatrix.size(); i++){
                if(DataMatrix[i][DataMatrix[i].size()-1] == class_label_A)
                    S_root_pos++;
                else
                    S_root_neg++;
    }

    int S_total= S_root_neg+ S_root_pos;


    //cout<<S_total<<endl;
   // cout<<S_root_pos<<" "<<S_root_neg<<endl;

    H_S= -S_root_pos * 1.0 /S_total * 1.0 *log2(S_root_pos * 1.0 /S_total * 1.0) - S_root_neg*1.0/S_total*1.0 * log2(S_root_neg*1.0/S_total*1.0);

   // cout<<H_S<<endl;


    // IG(S, Sx)= H(S) - (|S1|/|S|)* H(S1) -(|S2|/|S|)* H(S2) -(|S3|/|S|)* H(S3)....-(|Sn|/|S|)* H(Sn)
    // calculate entropy for the root node,  save the value of |S| , H(S)
    // choose each feature
    // for each feature, for each value of each feature , calculate entropy, save the value of |Sx|


    // collect the currently available columns/ features in a vector

    vector<int> available_features;

    for(int i=0; i< DataMatrix[0].size() -1; i++)
        available_features.push_back(DataMatrix[0][i]);

    int SIZE_AVAILABLE_FEATURES= available_features.size();

    int split_feature_index = -1 ; // kono feature nai, assumption

    int SIZE_DATA_MATRIX= DataMatrix.size();





    for(int i=0; i< SIZE_AVAILABLE_FEATURES; i++){

     //     cout<<"Current feature: "<<available_features[i]<<endl;

        //cout<<"HELLO"<<endl;

        int size_featureValues = FeatureMatrix[available_features[i]].size();
        int feature_index;


        for(int k=0; k < DataMatrix[0].size()-1; k++)
          {

           if(DataMatrix[0][k] == available_features[i])
            {
                   // cout<<"index found"<<endl;
                    feature_index= k;
                    break;
            }

          }

        vector<int> class_A_values;
        vector<int> class_B_values;

        for(int j=0; j< size_featureValues; j++){


            // ekhon proti ta particular feature value er jonno, classA ar class B data save korbo

            int current_feature_value= FeatureMatrix[available_features[i]][j];  // i'th feature's j'th value

            //age column/feature index ber koro(in DataMatrix), tar por,  ei value, jei jei instance e ase , oigula instance e explore korbo
            //oi instance theke class_A , class_B value collect korbo

            // k hocche feature index , to k-th column er value jodi current_feature_value hoy tahole , taholei er classA ar classB collect korbo
            int classACount=0;
            int classBCount=0;

            for(int k=1; k< SIZE_DATA_MATRIX; k++  ){
                // k-th instance er feature_index tomo feature
                if(DataMatrix[k][feature_index] == current_feature_value){

                    if(DataMatrix[k][SIZE_AVAILABLE_FEATURES] == class_label_A)
                        classACount++;
                    else
                        classBCount++;
                }
            }

            class_A_values.push_back(classACount); // YES <-> 1
            class_B_values.push_back(classBCount); // NO  <-> 0

        }

        // so, we will have all the classACount, and classBCount for all the values of a feature
        // time to calculate the entropies for each possible value of the feature and store it

        //vector<double> H_x;


        // class_A_values[k], class_B_values[k] has the values for ith value of feature i

        int size_T= class_A_values.size();

        double information_gain= H_S;

        for(int k=0; k<size_T ; k++){

            int pos= class_A_values[k];
            int neg= class_B_values[k];
            int total = pos + neg;

            double H_temp;

    //        cout<<"Pos: "<<pos<<" "<<"Neg: "<<neg<<" Total: "<<total<<endl;
            if(pos != total && neg!= total)
                H_temp = -pos * 1.0 /total * 1.0 *log2(pos * 1.0 /total * 1.0) - neg*1.0/total*1.0 * log2(neg*1.0/total*1.0);
            else
                H_temp=0;  // if pos == total or neg == total, then there is no randomness

        //    cout<<"Current feature: "<<available_features[i]<<endl;
          //  cout<< "Size of column names: "<< column_names.size()<<endl;
      //      cout<<"Feature: "<<column_names[available_features[i]]<<" Entropy: "<<H_temp<<endl;
           // H_x.push_back(H_temp);
           // H_temp hocche, k-th value of current feature er entropy

           information_gain-=  (total*1.0/S_total)*H_temp;

        }

     //      cout<<"Feature: "<<column_names[available_features[i]]<<": Information Gain: "<<information_gain<<" MAX information gain: "<< MAX_INFORMATION_GAIN <<endl;

           if( information_gain > MAX_INFORMATION_GAIN){
                MAX_INFORMATION_GAIN= information_gain;
             //   cout<<"SETTING: "<<column_names[i]<<endl;
                split_feature_index= available_features[i];
           }
    }
   // cout<<"Split Feature sent: "<< column_names[split_feature_index]<<endl;
    return split_feature_index;
}





TreeNode* createDecisionTree( TreeNode* parentNode, vector<vector<int> >& DataMatrix, vector<vector<int> >& FeatureMatrix ){


   //     cout<<"TEST"<<" "<<DataMatrix.size()<<endl;

        // detect when only one feature is left, so no further split is required


        //

        if( DataMatrix.size() == 1) // only the first row exists, that is, there is no data, all data has been trained
            {
                    return NULL;


            }
        // check if all the class labels are the same
        // data matrix er last column er sob element jodi eki cheharar hoy tahole hobe

        int class_label= DataMatrix[1][DataMatrix[1].size()-1];  // first data instance er last value
        int SIZE= DataMatrix.size();
        int SIZE_1= DataMatrix[1].size() -1;
        int flag_pure_node=true;
     //   PrintDataMatrix(DataMatrix);
        for(int i=2 ; i<SIZE; i++ ){

            if(DataMatrix[i][SIZE_1] != class_label ){
                flag_pure_node=false;
                break;
            }
        }

        // if true, then all the instances have same class label
        if(flag_pure_node){

            parentNode->isPureNode= true;
            parentNode->nodeLabel= class_label;
            return parentNode;
        }




    int split_feature= getSplitFeature(DataMatrix, FeatureMatrix);
   // cout<< " Split Feature: " <<split_feature<<" "<<column_names[split_feature] << endl;

    parentNode->split_feature= split_feature;

    for(int i =0; i< FeatureMatrix[split_feature].size(); i++) // feature er value gulo ase feature matrix e
    {


        ///constructing a child node, which is to be attached to the parent node , transition from parent to child happens with the nodeLabel= FeatureValue
        TreeNode* childNode= new TreeNode();

        // calculate majority here
        // ei feature er ei value e koyta true ar koyta false , eder moddhe majority , but equal hole , I will label it as
        int current_value= FeatureMatrix[split_feature][i];

        int classA, classB;
        classA= classB=0;

        //split_feature tomo column

        for(int p= 0; p< DataMatrix.size(); p++){

            if(DataMatrix[p][split_feature] == current_value){

                if(DataMatrix[p][DataMatrix[0].size()-1] == 1)
                    classA++;
                else
                    classB++;
            }
        }

        if(classA> classB)
            childNode->nodeLabel= classA;
        else if(classB> classA)
            childNode->nodeLabel= classB;
        else if(classB == classA)
            childNode->nodeLabel = classA;
        // assuming classA is the positive class



             //   cout<<"Split Feature: "<<column_names[split_feature]<<" Split value: "<< FeatureMatrix[split_feature][i]<<endl;
     //   PrintDataMatrix(DataMatrix);
        parentNode->isPureNode= false;
        parentNode->child_attributes.push_back(FeatureMatrix[split_feature][i]);
        vector<vector<int> > newDataMatrix= splitDataMatrix(DataMatrix, split_feature, FeatureMatrix[split_feature][i]);

   //     cout<<"--------------------------------------------------------------------------------"<<endl;
        parentNode->child_nodes.push_back( createDecisionTree(childNode, newDataMatrix, FeatureMatrix));

    }

    return parentNode;

}



int classifyNewDataInstance(TreeNode* root, vector<int> data_instance, vector<vector<int> > DataMatrix){

    int class_label;

    while(!root->isPureNode && !root->child_nodes.empty()){
          //  cout<<"MUST BE STUCK HERE."<<endl;

            int splitFeature= root->split_feature;
            int value= data_instance[splitFeature];

            // find out which child has this value
            int child_index;

            for(int i=0; i< root->child_attributes.size(); i++)
                if(root->child_attributes[i]==value)
                {
                    child_index=i;
                    break;
                }
           // cout<<"Child index: "<<child_index<<endl;

            root= root->child_nodes[child_index];
           // cout<<"Failed here?" <<endl;



            if(root == NULL)  // leaf node, not really leaf note, it's a node with
            {
                break;
            }

          //  cout<<"Hmm?"<<endl;

            class_label= root->nodeLabel; // BUT, eikhane to ase feature value

    }

    return class_label;

}



void PrintDataMatrix(vector<vector<int> > DataMatrix){


// Printing Data

        cout<<"Size of Current DataMatrix: "<< DataMatrix.size()<<endl;

        for(int i=0; i< DataMatrix[0].size(); i++)
            cout<<column_names[DataMatrix[0][i]]<<" ";
        cout<< endl;

        for(int i=0 ; i< DataMatrix.size(); i++){


            for(int j=0; j< DataMatrix[i].size(); j++)
                cout<<DataMatrix[i][j]<<" ";
            cout<<endl;
        }

}


void createUniqueAttributeFeatureMatrix(vector<vector<int> >& TrainingDataMatrix, vector<vector<int> >& uniqueAttributePerFeatureMatrix){

    int SIZE_col= TrainingDataMatrix[0].size();  // number of columns
         int SIZE_row= TrainingDataMatrix.size() ; // number of data instances



         for(int i=0; i< SIZE_col-1 ; i++){  // the last column has the class values, ignoring that


            vector<int> temp;
            map<int, int> flags;


            for(int j=1; j< SIZE_row; j++ ){  // cause row one just has the feature labels (in numbers)


                 if(flags.count(TrainingDataMatrix[j][i]) == 0){
                    temp.push_back(TrainingDataMatrix[j][i]);
                    flags[TrainingDataMatrix[j][i]]++;
                 }
            }

            uniqueAttributePerFeatureMatrix.push_back(temp);
            }

}


int main(void){




        ifstream myfile("Assignment1_data_set.csv");
       //   ifstream myfile("test_data.csv");
        if( !myfile.is_open()){
            cout<<"Data file not found."<<endl;
            exit(0);
        }

        vector<int> data;
        char delimeter;

        string data_line;
        int col_no=1;

              // labels for each column
        string parse_column_name;
        getline(myfile, parse_column_name);


/// parsing and storing the column labels, and determining number of features or columns
        while( parse_column_name.find(",") != string::npos){

            int position_delimeter = parse_column_name.find(",");
            column_names.push_back( string(parse_column_name.substr(0, position_delimeter)));
        //    cout<<parse_column_name.substr(0, position_delimeter)<<endl;
            parse_column_name.erase(0, position_delimeter+1);
            col_no++;
        }
        column_names.push_back(string(parse_column_name));

        cout<<"NUMBER OF FEATURES: "<<col_no<<endl;

        data.resize(col_no);

      //  if(myfile.is_open()){
        //    cout<<"YES"<<endl;
        //  }



//        cout<<"COLUMN NO: "<<col_no<<endl;

/// will use numeric values for each column
        for(int i=0; i<col_no; i++)
            data[i]=i;

        /// numbers in the first row correspond to features , in this case there are 9 features

        csvFileDataMatrix.push_back(data);
     //   uniqueAttributePerFeatureMatrix.push_back(data);


/// printing column names
//        for(int i=0 ; i< col_no; i++)
  //          cout<<column_names[i]<<" ";
    //    cout<<endl;


       // cout<<parse_column_name<<endl;


/// getting data instance and storing in a 2D vector row by row

        while( getline(myfile , data_line)){

                stringstream data_instance(data_line);
                for(int i=0; i<col_no ; i++){
                    data_instance>>data[i];
                    if(i != col_no)
                        data_instance>>delimeter;

                }

                csvFileDataMatrix.push_back(data);


        }


        // SPLIT csvFileDataMatrix into two parts, Training Data and Test Data

        int total_size= csvFileDataMatrix.size();

        //int test_data_size= ceil(1*.0 * total_size/5.0);
       // int training_data_size= floor((4.0* total_size)/5.0);


        // 1st 4/5 to training data , last 1/5 to testing data   4 1
        TrainingDataMatrix1.push_back(csvFileDataMatrix[0]);
        for(int i=0; i< (4* total_size)/5; i++)
        {
            TrainingDataMatrix1.push_back(csvFileDataMatrix[i]);
        }

        for(int i= (4*total_size)/5 ; i < total_size ; i++){

            TestDataMatrix1.push_back(csvFileDataMatrix[i]);
        }

        //first 3/5 to training data, middle 1/5 to testing data,  last 1/5 to training data 3 1 1

        for(int i=0; i< (3* total_size)/5 ; i++)
            TrainingDataMatrix2.push_back(csvFileDataMatrix[i]);

        for(int i=(3* total_size)/5 ; i< (4* total_size)/5 ; i++)
            TestDataMatrix2.push_back(csvFileDataMatrix[i]);

        for(int i=(4*total_size)/5; i< total_size; i++)
            TrainingDataMatrix2.push_back(csvFileDataMatrix[i]);


        // first 2/5 to training data , middle 1/5 to testing data, last 2/5 to training data  2 1 2


        for(int i=0; i< (2* total_size)/5 ; i++)
            TrainingDataMatrix3.push_back(csvFileDataMatrix[i]);

        for(int i=(2* total_size)/5 ; i< (3* total_size)/5 ; i++)
            TestDataMatrix3.push_back(csvFileDataMatrix[i]);

        for(int i=(3*total_size)/5; i< total_size; i++)
            TrainingDataMatrix3.push_back(csvFileDataMatrix[i]);


        //first 1/5 to training data , middle 1/5 to testing data , last 3/5 to training data 1 1 3


        for(int i=0; i< (1* total_size)/5 ; i++)
            TrainingDataMatrix4.push_back(csvFileDataMatrix[i]);

        for(int i=(1* total_size)/5 ; i< (2* total_size)/5 ; i++)
            TestDataMatrix4.push_back(csvFileDataMatrix[i]);

        for(int i=(2*total_size)/5; i< total_size; i++)
            TrainingDataMatrix4.push_back(csvFileDataMatrix[i]);

        // first 1/5 to TESTING data, last 4/5 to training data 1 5


        //setting the first feature row
        TrainingDataMatrix5.push_back(csvFileDataMatrix[0]);

        for(int i=1; i< (1* total_size)/5 +1 ; i++)
            TestDataMatrix5.push_back(csvFileDataMatrix[i]);

        for(int i=(1* total_size)/5 +1; i<total_size ; i++)
            TrainingDataMatrix5.push_back(csvFileDataMatrix[i]);


         // 0-th row has the attribute values for 0-th feature



         // creating a matrix that has different unique values per feature
         createUniqueAttributeFeatureMatrix(TrainingDataMatrix1, uniqueAttributePerFeatureMatrix1 );
         createUniqueAttributeFeatureMatrix(TrainingDataMatrix2, uniqueAttributePerFeatureMatrix2 );
         createUniqueAttributeFeatureMatrix(TrainingDataMatrix3, uniqueAttributePerFeatureMatrix3 );
         createUniqueAttributeFeatureMatrix(TrainingDataMatrix4, uniqueAttributePerFeatureMatrix4 );
         createUniqueAttributeFeatureMatrix(TrainingDataMatrix5, uniqueAttributePerFeatureMatrix5 );




// Printing Feature Matrix
//    for(int i=0; i<uniqueAttributePerFeatureMatrix.size(); i++){
//
//        for(int j=0; j< uniqueAttributePerFeatureMatrix[i].size(); j++){
//
//            cout<< uniqueAttributePerFeatureMatrix[i][j]<<" ";
//        }
//        cout<<endl;
//    }

// Printing Data
//
//        for(int i=0 ; i< csvFileDataMatrix.size(); i++){
//
//
//            for(int j=0; j< csvFileDataMatrix[i].size(); j++)
//                cout<<csvFileDataMatrix[i][j]<<" ";
//            cout<<endl;
//        }

    TreeNode * root1= new TreeNode();
    TreeNode * root2 = new TreeNode();
    TreeNode * root3 = new TreeNode();
    TreeNode *root4 = new TreeNode();
    TreeNode *root5 = new TreeNode();

   // PrintDataMatrix(csvFileDataMatrix);
   // cout<<"---------------------------"<<endl;
  //  vector<vector<int> > newDataMatrix = splitDataMatrix(csvFileDataMatrix, 0 , 0);

 //   PrintDataMatrix(newDataMatrix);

    createDecisionTree( root1, TrainingDataMatrix1, uniqueAttributePerFeatureMatrix1);


    createDecisionTree( root2, TrainingDataMatrix2, uniqueAttributePerFeatureMatrix2);
    createDecisionTree( root3, TrainingDataMatrix3, uniqueAttributePerFeatureMatrix3);
    createDecisionTree( root4, TrainingDataMatrix4, uniqueAttributePerFeatureMatrix4);
    createDecisionTree( root5, TrainingDataMatrix5, uniqueAttributePerFeatureMatrix5);

 //   cout<<"HERE? "<<endl;
//

    // factors to be calculated
    double TPR , TNR, PPV, NPV, FPR, FNR, FDR, ACC,   F1_score;
    // parameters for calculation

    double P, N, TP, TN, FP, FN;

    P= N = TP= TN  = FP = FN =0;

    for(int i=1; i< TestDataMatrix1.size(); i++){


        int predictedClass= classifyNewDataInstance(root1, TestDataMatrix1[i], TestDataMatrix1);
   //     cout<<"STUCK HERE?"<<endl;
        int actual_class=TestDataMatrix1[i][TestDataMatrix1[0].size()-1];


        if(actual_class ==1)
            P++;
        else if(actual_class==0)
            N++;

        if(predictedClass==1 && actual_class==1)
            TP++;
        else if(predictedClass==0 && actual_class==0)
            TN++;

        else if(predictedClass==0 && actual_class==1)
            FN++;
        else if(predictedClass==1 && actual_class==0)
            FP++;

    }

    TPR= TP/P*100;
    TNR= TN/N* 100;

    PPV= TP/(TP+FP)* 100;
    NPV= (TN)/ (TN+ FN)* 100;
    FPR= FP/N * 100;

    FDR= (FP)/(FP+TP)* 100;

    FNR= (FN)/P * 100;

    F1_score= 2*TP /(2*TP + FP + FN) * 100;

    ACC= (TP+ TN)/(P+N) * 100;

    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<"TruePosRate="<<TPR<<" TrueNegRate="<<TNR<<" PosPredictiveValue="<<PPV<<endl<<endl;
    cout<<"NegPredictivevalue="<<NPV<<" FalsePosRate="<<FPR<<" FalseNegRate= "<<FNR<<endl<<endl;
    cout<<"FalseDiscoveryRate="<<FDR<<" F1Score="<<F1_score<<" Accuracy="<<ACC<<endl<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;

    for(int i=1; i< TestDataMatrix2.size(); i++){

        int predictedClass= classifyNewDataInstance(root2, TestDataMatrix2[i], TestDataMatrix2);
        int actual_class=TestDataMatrix2[i][TestDataMatrix2[0].size()-1];


        if(actual_class ==1)
            P++;
        else if(actual_class==0)
            N++;

        if(predictedClass==1 && actual_class==1)
            TP++;
        else if(predictedClass==0 && actual_class==0)
            TN++;

        else if(predictedClass==0 && actual_class==1)
            FN++;
        else if(predictedClass==1 && actual_class==0)
            FP++;

    }

    TPR= TP/P*100;
    TNR= TN/N* 100;

    PPV= TP/(TP+FP)* 100;
    NPV= (TN)/ (TN+ FN)* 100;
    FPR= FP/N * 100;

    FDR= (FP)/(FP+TP)* 100;

    FNR= (FN)/P * 100;

    F1_score= 2*TP /(2*TP + FP + FN) * 100;

    ACC= (TP+ TN)/(P+N) * 100;


    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<"TruePosRate="<<TPR<<" TrueNegRate="<<TNR<<" PosPredictiveValue="<<PPV<<endl<<endl;
    cout<<"NegPredictivevalue="<<NPV<<" FalsePosRate="<<FPR<<" FalseNegRate= "<<FNR<<endl<<endl;
    cout<<"FalseDiscoveryRate="<<FDR<<" F1Score="<<F1_score<<" Accuracy="<<ACC<<endl<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;

    for(int i=1; i< TestDataMatrix3.size(); i++){

        int predictedClass= classifyNewDataInstance(root3, TestDataMatrix3[i], TestDataMatrix3);
   //     cout<<"STUCK HERE?"<<endl;
        int actual_class=TestDataMatrix3[i][TestDataMatrix3[0].size()-1];


        if(actual_class ==1)
            P++;
        else if(actual_class==0)
            N++;

        if(predictedClass==1 && actual_class==1)
            TP++;
        else if(predictedClass==0 && actual_class==0)
            TN++;

        else if(predictedClass==0 && actual_class==1)
            FN++;
        else if(predictedClass==1 && actual_class==0)
            FP++;

    }

    TPR= TP/P*100;
    TNR= TN/N* 100;

    PPV= TP/(TP+FP)* 100;
    NPV= (TN)/ (TN+ FN)* 100;
    FPR= FP/N * 100;

    FDR= (FP)/(FP+TP)* 100;

    FNR= (FN)/P * 100;

    F1_score= 2*TP /(2*TP + FP + FN) * 100;

    ACC= (TP+ TN)/(P+N) * 100;


    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<"TruePosRate="<<TPR<<" TrueNegRate="<<TNR<<" PosPredictiveValue="<<PPV<<endl<<endl;
    cout<<"NegPredictivevalue="<<NPV<<" FalsePosRate="<<FPR<<" FalseNegRate= "<<FNR<<endl<<endl;
    cout<<"FalseDiscoveryRate="<<FDR<<" F1Score="<<F1_score<<" Accuracy="<<ACC<<endl<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;


    for(int i=1; i< TestDataMatrix4.size(); i++){

//        cout<<"Test Data Instance: "<< i <<endl;
    //    for(int j=0; j<TestDataMatrix2[i].size(); j++){
      //          cout<<TestDataMatrix2[i][j]<<" ";
       // }
       // cout<<endl;

        int predictedClass= classifyNewDataInstance(root4, TestDataMatrix4[i], TestDataMatrix4);
   //     cout<<"STUCK HERE?"<<endl;
        int actual_class=TestDataMatrix4[i][TestDataMatrix4[0].size()-1];


        if(actual_class ==1)
            P++;
        else if(actual_class==0)
            N++;

        if(predictedClass==1 && actual_class==1)
            TP++;
        else if(predictedClass==0 && actual_class==0)
            TN++;

        else if(predictedClass==0 && actual_class==1)
            FN++;
        else if(predictedClass==1 && actual_class==0)
            FP++;

    }

    TPR= TP/P*100;
    TNR= TN/N* 100;

    PPV= TP/(TP+FP)* 100;
    NPV= (TN)/ (TN+ FN)* 100;
    FPR= FP/N * 100;

    FDR= (FP)/(FP+TP)* 100;

    FNR= (FN)/P * 100;

    F1_score= 2*TP /(2*TP + FP + FN) * 100;

    ACC= (TP+ TN)/(P+N) * 100;

    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<"TruePosRate="<<TPR<<" TrueNegRate="<<TNR<<" PosPredictiveValue="<<PPV<<endl<<endl;
    cout<<"NegPredictivevalue="<<NPV<<" FalsePosRate="<<FPR<<" FalseNegRate= "<<FNR<<endl<<endl;
    cout<<"FalseDiscoveryRate="<<FDR<<" F1Score="<<F1_score<<" Accuracy="<<ACC<<endl<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;


    for(int i=1; i< TestDataMatrix5.size(); i++){

//        cout<<"Test Data Instance: "<< i <<endl;
    //    for(int j=0; j<TestDataMatrix2[i].size(); j++){
      //          cout<<TestDataMatrix2[i][j]<<" ";
       // }
       // cout<<endl;

        int predictedClass= classifyNewDataInstance(root5, TestDataMatrix5[i], TestDataMatrix5);
        int actual_class=TestDataMatrix5[i][TestDataMatrix5[0].size()-1];

   //     cout<<"STUCK HERE?"<<endl;

        if(actual_class ==1)
            P++;
        else if(actual_class==0)
            N++;

        if(predictedClass==1 && actual_class==1)
            TP++;
        else if(predictedClass==0 && actual_class==0)
            TN++;

        else if(predictedClass==0 && actual_class==1)
            FN++;
        else if(predictedClass==1 && actual_class==0)
            FP++;

    }

    TPR= TP/P*100;
    TNR= TN/N* 100;

    PPV= TP/(TP+FP)* 100;
    NPV= (TN)/ (TN+ FN)* 100;
    FPR= FP/N * 100;

    FDR= (FP)/(FP+TP)* 100;

    FNR= (FN)/P * 100;

    F1_score= 2*TP /(2*TP + FP + FN) * 100;

    ACC= (TP+ TN)/(P+N) * 100;

    cout<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<"TruePosRate="<<TPR<<" TrueNegRate="<<TNR<<" PosPredictiveValue="<<PPV<<endl<<endl;
    cout<<"NegPredictivevalue="<<NPV<<" FalsePosRate="<<FPR<<" FalseNegRate= "<<FNR<<endl<<endl;
    cout<<"FalseDiscoveryRate="<<FDR<<" F1Score="<<F1_score<<" Accuracy="<<ACC<<endl<<endl;
    cout<<"---------------------------------------------------------------------------------------------------"<<endl;
    cout<<endl;

   // int splitFeatureIndex= getSplitFeature( TrainingDataMatrix, uniqueAttributePerFeatureMatrix);
   // cout<< "Split Feature: "<<column_names[splitFeatureIndex] << endl;
  //  vector<int> testData;

    //testData.push_back(8);
   // testData.push_back(7);
    //testData.push_back(5);
   // testData.push_back(10);
   // testData.push_back(7);
   // testData.push_back(9);
   // testData.push_back(5);
   // testData.push_back(5);

    //testData.push_back(1);

//    cout<<"Predicted class: "<< classifyNewDataInstance(root, testData, TrainingDataMatrix)<<endl;

    return 0;
}
