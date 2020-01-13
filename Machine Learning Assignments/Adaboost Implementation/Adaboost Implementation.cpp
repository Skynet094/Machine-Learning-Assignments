#include<bits/stdc++.h>

using namespace std;

vector < vector < int > > csvFileDataMatrix;
/// csv file data will be laid out as a 2D matrix , where each row correspond to each data instance
vector < double > alpha_weights;
vector < string > column_names;
int majority_label = -1;

void PrintDataMatrix(vector < vector < int > > DataMatrix);
/// defining information per node

struct TreeNode {

  int split_feature;
  vector < TreeNode * > child_nodes;
  vector < int > child_attributes; /// child_attribute[i] will be associated with child_nodes[i]
  bool isPureNode;
  int nodeLabel; /// node label if it's the leaf

};

vector < TreeNode * > composite_tree;

vector < vector < int > > splitDataMatrix(vector < vector < int > > DataMatrix, int splitFeature, int splitValue) {

  vector < vector < int > > newDataMatrix;

  // two things
  // one: do not include the column in the new DataMatrix
  //second: do not take the data instances where split_feature_value= splitValue
  int SIZE_row = DataMatrix.size(); // number of data instance
  int SIZE_col = DataMatrix[0].size(); // number of columns

  // jokhon ekta ekta kore column delete korbo, tokhon to , column shift hoye ek ghor bame ashbe,  FeatureMatrix er sathe mismatch hoye jabe , tai
  // but first row te feature er information thake, oi ta use kore determine korbo je kon index ta kon Feature er , ba Feature ta kon column e ase
  int featureIndex;

  for (int i = 0; i < DataMatrix[0].size(); i++) {
    if (DataMatrix[0][i] == splitFeature) {
      featureIndex = i;
      break;
    }
  }
  //  cout<<"Feature index: "<<featureIndex<<endl;
  // cout<<"Size ROW: "<< SIZE_row<<" "<<"SIZE COL: " <<SIZE_col<<endl;

  vector < int > TEMP;

  for (int i = 0; i < SIZE_col; i++) {
    if (i != featureIndex)
      TEMP.push_back(DataMatrix[0][i]);

  }

  newDataMatrix.push_back(TEMP);

  for (int i = 1; i < SIZE_row; i++) {

    vector < int > temp;
    // skip row if splitValue== featureValue
    if (DataMatrix[i][featureIndex] != splitValue)
      continue;
    // cout<<"YES"<<endl;

    for (int j = 0; j < SIZE_col; j++) {
      if (j != featureIndex) // skip the feature column
        temp.push_back(DataMatrix[i][j]);

    }
    // cout<<"YES?"<<endl;
    newDataMatrix.push_back(temp);
  }

  return newDataMatrix;
}

int getSplitFeature(vector < vector < int > > DataMatrix, vector < vector < int > > FeatureMatrix) {

  ///calculate information gain

  // IG(S, Sx)= H(S) - (|S1|/|S|)* H(S1) -(|S2|/|S|)* H(S2) -(|S3|/|S|)* H(S3)....-(|Sn|/|S|)* H(Sn)
  // calculate entropy for the root node,  save the value of |S| , H(S)
  // choose each feature
  // for each feature, for each value of each feature , calculate entropy, save the value of |Sx|

  double S_root_pos;
  double S_root_neg;
  double H_S;
  map < int, int > count_class_label;
  double MAX_INFORMATION_GAIN = -INT_MAX * 1.0;
  int class_label_A = 1; //picking one label, malignant

  for (int i = 1; i < DataMatrix.size(); i++) {
    if (DataMatrix[i][DataMatrix[i].size() - 1] == class_label_A)
      S_root_pos++;
    else
      S_root_neg++;
  }

  int S_total = S_root_neg + S_root_pos;

  //cout<<S_total<<endl;
  // cout<<S_root_pos<<" "<<S_root_neg<<endl;

  H_S = -S_root_pos * 1.0 / S_total * 1.0 * log2(S_root_pos * 1.0 / S_total * 1.0) - S_root_neg * 1.0 / S_total * 1.0 * log2(S_root_neg * 1.0 / S_total * 1.0);

  // cout<<H_S<<endl;

  // IG(S, Sx)= H(S) - (|S1|/|S|)* H(S1) -(|S2|/|S|)* H(S2) -(|S3|/|S|)* H(S3)....-(|Sn|/|S|)* H(Sn)
  // calculate entropy for the root node,  save the value of |S| , H(S)
  // choose each feature
  // for each feature, for each value of each feature , calculate entropy, save the value of |Sx|

  // collect the currently available columns/ features in a vector

  vector < int > available_features;

  for (int i = 0; i < DataMatrix[0].size() - 1; i++)
    available_features.push_back(DataMatrix[0][i]);

  int SIZE_AVAILABLE_FEATURES = available_features.size();

  int split_feature_index = -1; // kono feature nai, assumption

  int SIZE_DATA_MATRIX = DataMatrix.size();

  for (int i = 0; i < SIZE_AVAILABLE_FEATURES; i++) {

    //     cout<<"Current feature: "<<available_features[i]<<endl;

    //cout<<"HELLO"<<endl;

    int size_featureValues = FeatureMatrix[available_features[i]].size();
    int feature_index;

    for (int k = 0; k < DataMatrix[0].size() - 1; k++) {

      if (DataMatrix[0][k] == available_features[i]) {
        // cout<<"index found"<<endl;
        feature_index = k;
        break;
      }

    }

    vector < int > class_A_values;
    vector < int > class_B_values;

    for (int j = 0; j < size_featureValues; j++) {

      // ekhon proti ta particular feature value er jonno, classA ar class B data save korbo

      int current_feature_value = FeatureMatrix[available_features[i]][j]; // i'th feature's j'th value

      //age column/feature index ber koro(in DataMatrix), tar por,  ei value, jei jei instance e ase , oigula instance e explore korbo
      //oi instance theke class_A , class_B value collect korbo

      // k hocche feature index , to k-th column er value jodi current_feature_value hoy tahole , taholei er classA ar classB collect korbo
      int classACount = 0;
      int classBCount = 0;

      for (int k = 1; k < SIZE_DATA_MATRIX; k++) {
        // k-th instance er feature_index tomo feature
        if (DataMatrix[k][feature_index] == current_feature_value) {

          if (DataMatrix[k][SIZE_AVAILABLE_FEATURES] == class_label_A)
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

    int size_T = class_A_values.size();

    double information_gain = H_S;

    for (int k = 0; k < size_T; k++) {

      int pos = class_A_values[k];
      int neg = class_B_values[k];
      int total = pos + neg;

      double H_temp;

      //        cout<<"Pos: "<<pos<<" "<<"Neg: "<<neg<<" Total: "<<total<<endl;
      if (pos != total && neg != total)
        H_temp = -pos * 1.0 / total * 1.0 * log2(pos * 1.0 / total * 1.0) - neg * 1.0 / total * 1.0 * log2(neg * 1.0 / total * 1.0);
      else
        H_temp = 0; // if pos == total or neg == total, then there is no randomness

      //    cout<<"Current feature: "<<available_features[i]<<endl;
      //  cout<< "Size of column names: "<< column_names.size()<<endl;
      //      cout<<"Feature: "<<column_names[available_features[i]]<<" Entropy: "<<H_temp<<endl;
      // H_x.push_back(H_temp);
      // H_temp hocche, k-th value of current feature er entropy

      information_gain -= (total * 1.0 / S_total) * H_temp;

    }

    //      cout<<"Feature: "<<column_names[available_features[i]]<<": Information Gain: "<<information_gain<<" MAX information gain: "<< MAX_INFORMATION_GAIN <<endl;

    if (information_gain > MAX_INFORMATION_GAIN) {
      MAX_INFORMATION_GAIN = information_gain;
      //   cout<<"SETTING: "<<column_names[i]<<endl;
      split_feature_index = available_features[i];
    }
  }
  // cout<<"Split Feature sent: "<< column_names[split_feature_index]<<endl;
  return split_feature_index;
}

void DecisionTreeStump(int depthLimit, TreeNode * parentNode, vector < vector < int > > & DataMatrix, vector < vector < int > > & FeatureMatrix) {

  //        cout<<"TEST"<<"****************************************************************************"<<DataMatrix.size()<<endl;

  // detect when only one feature is left, so no further split is required

  //

  if (DataMatrix.size() == 1) // only the first row exists, that is, there is no data, all data has been trained
  {
    return;

  }
  // check if all the class labels are the same
  // data matrix er last column er sob element jodi eki cheharar hoy tahole hobe

  int class_label = DataMatrix[1][DataMatrix[1].size() - 1]; // first data instance er last value
  int SIZE = DataMatrix.size();
  int SIZE_1 = DataMatrix[1].size() - 1;
  int flag_pure_node = true;
  //   PrintDataMatrix(DataMatrix);
  for (int i = 2; i < SIZE; i++) {

    if (DataMatrix[i][SIZE_1] != class_label) {
      flag_pure_node = false;
      break;
    }
  }

  // if true, then all the instances have same class label
  if (flag_pure_node) {

    parentNode - > isPureNode = true;
    parentNode - > nodeLabel = class_label;
    return;
  }

  int split_feature = getSplitFeature(DataMatrix, FeatureMatrix);
  //  cout<< " Split Feature: " <<split_feature<<" "<<column_names[split_feature] << endl;

  parentNode - > split_feature = split_feature;

  for (int i = 0; i < FeatureMatrix[split_feature].size(); i++) // feature er value gulo ase feature matrix e
  {

    ///constructing a child node, which is to be attached to the parent node , transition from parent to child happens with the nodeLabel= FeatureValue
    TreeNode * childNode = new TreeNode();

    // calculate majority here
    // ei feature er ei value e koyta true ar koyta false , eder moddhe majority , but equal hole , I will label it as
    int current_value = FeatureMatrix[split_feature][i];

    int classA, classB;
    classA = classB = 0;

    //split_feature tomo column

    for (int p = 1; p < DataMatrix.size(); p++) {

      if (DataMatrix[p][split_feature] == current_value) {

        if (DataMatrix[p][DataMatrix[0].size() - 1] == 1)
          classA++;
        else
          classB++;
      }
    }

    if (classA > classB)
      childNode - > nodeLabel = 1;
    else if (classB > classA)
      childNode - > nodeLabel = 0;
    else if (classB == classA)
      childNode - > nodeLabel = 1;
    // assuming classA is the positive class
    // cout<<"CHILD NODE LABEL: "<<childNode->nodeLabel<<" HERE LIES GOOD INFORMATION"<<endl;

    //               cout<<"Split Feature: "<<column_names[split_feature]<<" Split value: "<< FeatureMatrix[split_feature][i]<<endl;
    //   PrintDataMatrix(DataMatrix);
    parentNode - > isPureNode = false;
    parentNode - > child_attributes.push_back(FeatureMatrix[split_feature][i]);
    vector < vector < int > > newDataMatrix = splitDataMatrix(DataMatrix, split_feature, FeatureMatrix[split_feature][i]);

    //     cout<<"--------------------------------------------------------------------------------"<<endl;
    parentNode - > child_nodes.push_back(childNode);

  }

  //   cout<<" Is this working? parent_node= "<<parentNode->child_nodes.size()<<" "<<endl;

  return;

}

int classifyNewDataInstance(TreeNode * root, vector < int > data_instance) {

  int class_label;

  while (!root - > child_nodes.empty()) {
    //   cout<<"MUST BE STUCK HERE."<<endl;

    int splitFeature = root - > split_feature;
    //     cout<<"SPLIT FEATURE: "<<splitFeature<<endl;

    int value = data_instance[splitFeature];
    //       cout<<"DATA INSTANCE SPLIT VALUE "<<value<<endl;
    // find out which child has this value
    int child_index = -1;

    // cout<<"Child attribute size= "<<root->child_attributes.size()<<endl;

    //  cout<<"SPLIT VALUES: "<<endl<<endl;
    for (int i = 0; i < root - > child_attributes.size(); i++) {
      //  cout<< root->child_attributes[i]<<endl;
      if (root - > child_attributes[i] == value) {
        child_index = i;
        break;
      }
    }

    // cout<<"SPLIT VALUES END"<<endl;
    //cout<<endl<<endl;

    //cout<<"Child index: "<<child_index<<endl;

    if (child_index == -1)
      return 1;

    root = root - > child_nodes[child_index];
    //    cout<<"Failed here?" <<endl;

    if (root == NULL) // leaf node, not really leaf note, it's a node with
    {
      break;
    }

    //  cout<<"Hmm?"<<endl;

    class_label = root - > nodeLabel; // BUT, eikhane to ase feature value
    return class_label;

  }

  // cout<<"SENDING CLASS LABEL: "<<class_label<<endl;
  return class_label;

}

void PrintDataMatrix(vector < vector < int > > DataMatrix) {

  // Printing Data

  cout << "Size of Current DataMatrix: " << DataMatrix.size() << endl;

  for (int i = 0; i < DataMatrix[0].size(); i++)
    cout << column_names[DataMatrix[0][i]] << " ";
  cout << endl;

  for (int i = 0; i < DataMatrix.size(); i++) {

    for (int j = 0; j < DataMatrix[i].size(); j++)
      cout << DataMatrix[i][j] << " ";
    cout << endl;
  }

}

int ID3_classifyNewDataInstance(TreeNode * root, vector < int > data_instance) {

  int class_label;

  while (!root - > isPureNode && !root - > child_nodes.empty()) {
    //  cout<<"MUST BE STUCK HERE."<<endl;

    int splitFeature = root - > split_feature;
    int value = data_instance[splitFeature];

    // find out which child has this value
    int child_index = -1;

    for (int i = 0; i < root - > child_attributes.size(); i++)
      if (root - > child_attributes[i] == value) {
        child_index = i;
        break;
      }

    if (child_index == -1)
      return 1;

    // cout<<"Child index: "<<child_index<<endl;

    root = root - > child_nodes[child_index];
    // cout<<"Failed here?" <<endl;

    if (root == NULL) // leaf node, not really leaf note, it's a node with
    {
      break;
    }

    //  cout<<"Hmm?"<<endl;

    class_label = root - > nodeLabel; // BUT, eikhane to ase feature value

  }

  return class_label;

}

TreeNode * ID3_DecisionTree(TreeNode * parentNode, vector < vector < int > > & DataMatrix, vector < vector < int > > & FeatureMatrix) {

  //     cout<<"TEST"<<" "<<DataMatrix.size()<<endl;

  // detect when only one feature is left, so no further split is required

  //

  if (DataMatrix.size() == 1) // only the first row exists, that is, there is no data, all data has been trained
  {
    return NULL;

  }
  // check if all the class labels are the same
  // data matrix er last column er sob element jodi eki cheharar hoy tahole hobe

  int class_label = DataMatrix[1][DataMatrix[1].size() - 1]; // first data instance er last value
  int SIZE = DataMatrix.size();
  int SIZE_1 = DataMatrix[1].size() - 1;
  int flag_pure_node = true;
  //   PrintDataMatrix(DataMatrix);
  for (int i = 2; i < SIZE; i++) {

    if (DataMatrix[i][SIZE_1] != class_label) {
      flag_pure_node = false;
      break;
    }
  }

  // if true, then all the instances have same class label
  if (flag_pure_node) {

    parentNode - > isPureNode = true;
    parentNode - > nodeLabel = class_label;
    return parentNode;
  }

  int split_feature = getSplitFeature(DataMatrix, FeatureMatrix);
  // cout<< " Split Feature: " <<split_feature<<" "<<column_names[split_feature] << endl;

  parentNode - > split_feature = split_feature;

  for (int i = 0; i < FeatureMatrix[split_feature].size(); i++) // feature er value gulo ase feature matrix e
  {

    ///constructing a child node, which is to be attached to the parent node , transition from parent to child happens with the nodeLabel= FeatureValue
    TreeNode * childNode = new TreeNode();

    // calculate majority here
    // ei feature er ei value e koyta true ar koyta false , eder moddhe majority , but equal hole , I will label it as
    int current_value = FeatureMatrix[split_feature][i];

    int classA, classB;
    classA = classB = 0;

    //split_feature tomo column

    for (int p = 0; p < DataMatrix.size(); p++) {

      if (DataMatrix[p][split_feature] == current_value) {

        if (DataMatrix[p][DataMatrix[0].size() - 1] == 1)
          classA++;
        else
          classB++;
      }
    }

    if (classA > classB)
      childNode - > nodeLabel = 1;
    else if (classB > classA)
      childNode - > nodeLabel = 0;
    else if (classB == classA)
      childNode - > nodeLabel = 1;
    // assuming classA is the positive class

    //   cout<<"Split Feature: "<<column_names[split_feature]<<" Split value: "<< FeatureMatrix[split_feature][i]<<endl;
    //   PrintDataMatrix(DataMatrix);
    parentNode - > isPureNode = false;
    parentNode - > child_attributes.push_back(FeatureMatrix[split_feature][i]);
    vector < vector < int > > newDataMatrix = splitDataMatrix(DataMatrix, split_feature, FeatureMatrix[split_feature][i]);

    //     cout<<"--------------------------------------------------------------------------------"<<endl;
    parentNode - > child_nodes.push_back(ID3_DecisionTree(childNode, newDataMatrix, FeatureMatrix));

  }

  return parentNode;

}

void createUniqueAttributeFeatureMatrix(vector < vector < int > > & TrainingDataMatrix, vector < vector < int > > & uniqueAttributePerFeatureMatrix) {

  int SIZE_col = TrainingDataMatrix[0].size(); // number of columns
  int SIZE_row = TrainingDataMatrix.size(); // number of data instances

  //   cout<<endl<<endl;
  // cout<<"Training Dataset size= "<<TrainingDataMatrix.size()<<endl;
  //  cout<<endl<<endl;

  for (int i = 0; i < SIZE_col - 1; i++) { // the last column has the class values, ignoring that

    vector < int > temp;
    map < int, int > flags;

    for (int j = 1; j < SIZE_row; j++) { // cause row one just has the feature labels (in numbers)

      if (flags.count(TrainingDataMatrix[j][i]) == 0) {
        temp.push_back(TrainingDataMatrix[j][i]);
        flags[TrainingDataMatrix[j][i]]++;
      }
    }

    uniqueAttributePerFeatureMatrix.push_back(temp);
  }

}

void reSample(vector < vector < int > > DataSet, vector < double > weight_vector, vector < vector < int > > & resampledDataSet) {

  //        cout<<"ENTERED: resampling "<<endl;

  vector < double > cumulative_weight;
  cumulative_weight.resize(weight_vector.size() + 1);

  cumulative_weight.push_back(0);

  //cout<<endl<<endl;
  // cout<<"Cumulative weight size: "<<cumulative_weight.size()<< "weight_vector_size=  "<< weight_vector.size()<<endl;
  // cout<<"Resampled data size = "<< resampledDataSet.size()<<" Dataset size= "<<DataSet.size()<<endl;
  // cout<<endl<<endl;

  for (int i = 0; i < weight_vector.size(); i++) {

    cumulative_weight[i + 1] = cumulative_weight[i] + weight_vector[i];

  }

  int N = DataSet.size();

  for (int i = 0; i < N - 1; i++) {

    double random_value = (rand() * 1.0 / RAND_MAX * 1.0);

    for (int j = 0; j < N - 1; j++) {

      if (random_value < cumulative_weight[j + 1]) {
        //         cout<<j<<endl;
        resampledDataSet.push_back(DataSet[j + 1]); // skip first row, as it has feature labels
        break;

      }
    }

  }
  return;

}

// stores the hypothesis tree in composite tree and

void AdaBoost(int T, vector < vector < int > > DataSet, vector < vector < int > > FeatureMatrix) {

  //  cout<<"************************************ADABOOST***************************************"<<endl;
  // cout<<"TEST."<<endl;

  vector < double > weight_vector;

  int N = DataSet.size(); // first row is feature only , tar mane, N-1 hocche, final index

  weight_vector.resize(N + 1); // N-2 hocche final index

  // initializing weights

  for (int i = 0; i < N - 1; i++) { // N-1 ta data point

    weight_vector[i] = 1.0 / (N - 1) * 1.0;
  }

  //cout<<"1/N="<<1.0/N*1.0<<endl;

  vector < vector < int > > resampledDataSet, uniqueFeatureMatrix;

  //  cout<<DataSet.size()<<endl;

  //    PrintDataMatrix(DataSet);
  int i = 0;

  while (i < T) {
    //    cout<<"ENTER THE CHAOS"<<endl;

    //    cout<<"T=====" <<i<<endl;

    // data re-sampling here
    if (!resampledDataSet.empty())
      resampledDataSet.clear();

    resampledDataSet.push_back(DataSet[0]);
    reSample(DataSet, weight_vector, resampledDataSet);
    uniqueFeatureMatrix.clear();

    //cout<<"HELLO"<<endl;
    //  cout<<"RESAMPLE"<<endl;

    //   cout<<"moment of truth"<<endl;
    createUniqueAttributeFeatureMatrix(resampledDataSet, uniqueFeatureMatrix);

    TreeNode * decisionTree = new TreeNode();

    //   cout<<" Moment of truth "<<endl;
    //   cout<<"-------------------------------------------------------------------------------------------------------"<<endl;
    //  cout<<"Size of data set= "<< DataSet.size()<<" Size of resampled data: "<<resampledDataSet.size()<<" size Of unique feature= "<<uniqueFeatureMatrix.size()<<endl;
    //  cout<<"Size of weight vector= " <<weight_vector.size()<<" "<<"Size of Alpha vectors: "<<alpha_weights.size() <<endl;
    //  cout<<"-------------------------------------------------------------------------------------------------------"<<endl;

    DecisionTreeStump(0, decisionTree, resampledDataSet, uniqueFeatureMatrix);
    //        cout<<"HELLO?"<<endl;
    //  cout<<"Moment of truth passed"<<endl;

    composite_tree.push_back(decisionTree); // pushing the tree, will use later for classification

    double Et = 0;

    //  cout<<" Start: "<<endl;

    for (int j = 1; j < N; j++) {

      int predicted_class = classifyNewDataInstance(decisionTree, DataSet[j]);
      int actual_class = DataSet[j][DataSet[0].size() - 1];

      // cout<<"Actual class= "<<actual_class<<" predicted class= "<<predicted_class<<endl;

      if (predicted_class != actual_class) {
        Et += weight_vector[j - 1];
        //     cout<<"TROLOLOLO"<<endl;
      }

      //  else
      //    cout<<"Correct prediction"<<endl;
    }

    // cout<<"End."<<endl;

    //   cout<<"-----"<<endl;
    //  cout<<"ET = "<<Et<<endl;
    // cout<< (1-Et)*1.0 /Et*1.0 <<endl;

    //   if(Et< 0.49999) {

    // cout<<"************************************true*******************************************"<<endl;
    //  }
    //  else
    //    continue;

    double alpha = 0.5 * log((1 - Et) * 1.0 / Et * 1.0);
    //     cout<<alpha<< endl;
    //  cout<<"-----"<<endl<<endl;
    alpha_weights.push_back(alpha);

    //   cout<<"Value of alpha: "<<alpha<<" Value of error: "<<Et<<endl;

    //          cout<<"-------------------------"<<endl<<endl;
    //        cout<<"weights before"<<endl;
    //      for(int j=0; j< weight_vector.size(); j++)
    //        cout<<weight_vector[i]<<endl;
    //  cout<<"weights before ends"<<endl;

    //cout<<"THA FACK"<<endl;

    for (int j = 1; j < N; j++) {

      int predicted_class = classifyNewDataInstance(decisionTree, DataSet[j]);
      int actual_class = DataSet[j][DataSet[0].size() - 1];

      if (predicted_class != actual_class) {
        weight_vector[j - 1] = weight_vector[j - 1] * exp(alpha);
        //  cout<<"FALSE"<<" EXP: "<< weight_vector[j-1]<<" index= "<<j-1<<endl;
        // cout<<" ";
      } else if (predicted_class == actual_class) {
        //cout<< "EXP "<<exp(-1* alpha)<<endl;
        weight_vector[j - 1] = weight_vector[j - 1] * exp(-1 * alpha);
        //  cout<<" ";
        //  cout<<"TRUE"<<" EXP: "<<weight_vector[j-1]<<endl;

      }

    }
    //    cout<<endl;

    double total_weight = 0;

    for (int p = 0; p < weight_vector.size(); p++)
      total_weight += weight_vector[p];

    for (int p = 0; p < weight_vector.size(); p++)
      weight_vector[p] = weight_vector[p] * 1.0 / total_weight * 1.0;

    //      cout<<"weights after"<<endl;
    //    for(int j=0; j< weight_vector.size(); j++)
    //      cout<<weight_vector[j]<<endl;
    //   cout<<"weights after ends"<<endl;
    //  cout<<"-------------------------"<<endl<<endl;

    i++;
  }

}

int test_hypothesis_tree(vector < TreeNode * > decisionTreeCollection, vector < double > weights, vector < int > dataInstance) {

  int predicted_class = -1;
  double value = 0;
  // cout<<" Decision tree number: "<< decisionTreeCollection.size()<<endl;

  for (int i = 0; i < decisionTreeCollection.size(); i++) {

    TreeNode * node = decisionTreeCollection[i];

    predicted_class = classifyNewDataInstance(node, dataInstance);

    if (predicted_class == 1)
      value += alpha_weights[i];
    else if (predicted_class == 0)
      value -= alpha_weights[i];

  }

  if (value > 0)
    return 1;
  else
    return 0;

}

int main(void) {

  srand(time(NULL));

  ifstream myfile("Assignment1_data_set.csv");
  //   ifstream myfile("test_data.csv");
  if (!myfile.is_open()) {
    cout << "Data file not found." << endl;
    exit(0);
  }

  vector < int > data;
  char delimeter;

  string data_line;
  int col_no = 1;

  // labels for each column
  string parse_column_name;
  getline(myfile, parse_column_name);

  /// parsing and storing the column labels, and determining number of features or columns
  while (parse_column_name.find(",") != string::npos) {

    int position_delimeter = parse_column_name.find(",");
    column_names.push_back(string(parse_column_name.substr(0, position_delimeter)));
    //    cout<<parse_column_name.substr(0, position_delimeter)<<endl;
    parse_column_name.erase(0, position_delimeter + 1);
    col_no++;
  }
  column_names.push_back(string(parse_column_name));

  cout << "NUMBER OF FEATURES: " << col_no << endl;

  data.resize(col_no);

  //  if(myfile.is_open()){
  //    cout<<"YES"<<endl;
  //  }

  //        cout<<"COLUMN NO: "<<col_no<<endl;

  /// will use numeric values for each column
  for (int i = 0; i < col_no; i++)
    data[i] = i;

  /// numbers in the first row correspond to features , in this case there are 9 features

  csvFileDataMatrix.push_back(data);
  //   uniqueAttributePerFeatureMatrix.push_back(data);

  /// printing column names
  //        for(int i=0 ; i< col_no; i++)
  //          cout<<column_names[i]<<" ";
  //    cout<<endl;

  // cout<<parse_column_name<<endl;

  /// getting data instance and storing in a 2D vector row by row

  while (getline(myfile, data_line)) {

    stringstream data_instance(data_line);
    for (int i = 0; i < col_no; i++) {
      data_instance >> data[i];
      if (i != col_no)
        data_instance >> delimeter;

    }

    csvFileDataMatrix.push_back(data);

  }

  // GENERALIZED WAY OF K FOLDING  5, 10 , 20 , one leave out cross folding

  // SPLIT csvFileDataMatrix into two parts, Training Data and Test Data

  int total_size = csvFileDataMatrix.size();

  // K FOLD Cross validation , data generation

  cout << "Total size: " << total_size << endl;

  vector < vector < int > > dataTrain, dataTest, uniqueAttributePerFeatureMatrix;

  int round_number = 5;
  vector < int > round_numbers;

  round_numbers.push_back(5);
  round_numbers.push_back(10);
  round_numbers.push_back(20);
  round_numbers.push_back(30);
  //        round_numbers.push_back(csvFileDataMatrix.size() -1);  // leave one out

  int data_size = csvFileDataMatrix.size() - 1;

  for (int t = 0; t < round_numbers.size(); t++) {

    cout << "-----------------------------------------------------------" << endl;

    cout << "*************Accuracies for round number = " << round_numbers[t] << "******************" << endl;

    for (int i = 0; i < 3; i++) {

      round_number = round_numbers[t];

      int k = 5 * pow(2, i);

      if (i == 3)
        k = data_size - 1;

      // cout<<k<<" Fold cross validation, indexing "<<endl;

      int train_start1, train_start2, train_end1, train_end2;
      int test_start, test_end;

      int one_fragment = int((total_size - 1) * 1.0 / k * 1.0);

      test_start = 1;
      test_end = one_fragment;

      // cout<<"Size of one fragment: "<< one_fragment <<endl;
      //   cout<<"---------------------------------------------------------"<<endl<<endl<<endl;

      double average_accuracy_boosting = 0.0;
      double average_accuracy_ID3 = 0.0;
      double average_accuracy_DStump = 0.0;

      for (int j = 0; j < k; j++) {

        dataTrain.clear();
        dataTest.clear();
        uniqueAttributePerFeatureMatrix.clear();

        dataTrain.push_back(csvFileDataMatrix[0]);

        //cout<<"HELLO"<<endl;

        if (test_start != 1) {
          train_start1 = 1;
          train_end1 = test_start - 1;

          train_start2 = test_end + 1;
          train_end2 = total_size - 1;

        } else if (test_start == 1) {

          train_start1 = test_end + 1;
          train_start2 = train_start1;
          train_end1 = total_size - 1;
          train_end2 = train_end1;
        }

        if (train_start1 != train_start2) {

          // mane training data
          for (int m = train_start1; m <= train_end1; m++)
            dataTrain.push_back(csvFileDataMatrix[m]);

          for (int m = train_start2; m <= train_end2; m++)
            dataTrain.push_back(csvFileDataMatrix[m]);

        } else {

          for (int m = train_start1; m <= train_end1; m++)
            dataTrain.push_back(csvFileDataMatrix[m]);

        }

        for (int m = test_start; m <= test_end; m++)
          dataTest.push_back(csvFileDataMatrix[m]);

        createUniqueAttributeFeatureMatrix(dataTrain, uniqueAttributePerFeatureMatrix);

        // AdaBoost(round_number, dataTrain, uniqueAttributePerFeatureMatrix);

        // data boost will store the decision trees in that vector
        // using test data, obtain accuracy

        // composite tree is the output
        //  cout<<"HI!!!"<<endl;

        alpha_weights.clear();
        composite_tree.clear();

        AdaBoost(round_number, dataTrain, uniqueAttributePerFeatureMatrix);

        TreeNode * Id3_node = new TreeNode();
        TreeNode * stump_node = new TreeNode();

        ID3_DecisionTree(Id3_node, dataTrain, uniqueAttributePerFeatureMatrix);
        DecisionTreeStump(0, stump_node, dataTrain, uniqueAttributePerFeatureMatrix);
        //test_hypothesis_tree(composite_tree, alpha_weights,  )

        int correct_classification_boosting = 0;
        int correct_classification_id3 = 0;
        int correct_classification_decision_stump = 0;
        int total_test_instance = dataTest.size();

        int predicted_class, actual_class;

        for (int i = 0; i < dataTest.size(); i++) {

          //TESTING: boosting
          predicted_class = test_hypothesis_tree(composite_tree, alpha_weights, dataTest[i]);

          actual_class = dataTest[i][dataTest[0].size() - 1];

          if (predicted_class == actual_class)
            correct_classification_boosting++;
          //TESTING: ID3

          //          cout<< test_hypothesis_tree(composite_tree, alpha_weights, dataTest[i]) << " "<<ID3_classifyNewDataInstance(Id3_node, dataTest[i])<<" "<<classifyNewDataInstance(stump_node, dataTest[i])<<endl;
          predicted_class = ID3_classifyNewDataInstance(Id3_node, dataTest[i]);

          if (predicted_class == actual_class)
            correct_classification_id3++;

          predicted_class = classifyNewDataInstance(stump_node, dataTest[i]);

          if (predicted_class == actual_class)
            correct_classification_decision_stump++;

          //TESTING : Decision stump
          //cout<<predicted_class<<" "<<actual_class<<endl;

        }

        average_accuracy_boosting += (correct_classification_boosting * 1.0 / total_test_instance * 1.0);

        average_accuracy_ID3 += (correct_classification_id3 * 1.0 / total_test_instance * 1.0);
        average_accuracy_DStump += (correct_classification_decision_stump * 1.0 / total_test_instance * 1.0);
        test_start += one_fragment;
        test_end += one_fragment;

        //     break;

      }
      if (k != data_size - 1) {
        cout << "Average accuracy(Boosting) for K=" << k << " fold cross validation " << (average_accuracy_boosting) * 100 / k * 1.0 << "%" << endl;
        cout << "Average accuracy(ID3) for K=" << k << " fold cross validation " << (average_accuracy_ID3) * 100 / k * 1.0 << "%" << endl;
        cout << "Average accuracy(Decision Stump) for K=" << k << " fold cross validation " << (average_accuracy_DStump) * 100 / k * 1.0 << "%" << endl;
      } else {

        cout << "Average accuracy(Boosting) for leave one out: " << (average_accuracy_boosting) * 100 / k * 1.0 << "%" << endl;
        cout << "Average accuracy(ID3) for leave one out: " << (average_accuracy_ID3) * 100 / k * 1.0 << "%" << endl;
        cout << "Average accuracy(Decision Stump) for leave one out: " << (average_accuracy_DStump) * 100 / k * 1.0 << "%" << endl;
      }
      cout << endl;
      //  break;
    }
    cout << "---------------------------------------------------------" << endl << endl << endl;
  }

  //    createUniqueAttributeFeatureMatrix(TrainingDataMatrix1, uniqueAttributePerFeatureMatrix1 );
  // TreeNode * root1= new TreeNode();
  //  createDecisionTree( 0,root1, TrainingDataMatrix1, uniqueAttributePerFeatureMatrix1);
  return 0;
}