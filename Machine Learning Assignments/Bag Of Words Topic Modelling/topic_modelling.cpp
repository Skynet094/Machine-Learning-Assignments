#include<bits/stdc++.h>

using namespace std;
map < int, vector < string > > Document_collection;

int find_document(int word_num) {
  int num;

  int size_D = Document_collection.size();
  int words_size = Document_collection[0].size();

  for (int i = 1; i < size_D + 1; i++) { // when i== size_D,  check will occur against i=0 to i=size_D-1

    if (word_num < words_size) {

      return i - 1;
    }

    words_size += Document_collection[i].size();

  }

  return -1;

}

struct data {

  long long int number;
  size_t index;

  data(long long int num, size_t in ) {
    number = num;
    index = in ;
  }
};

struct by_number {
  bool operator()(data
    const & left, data
    const & right) {
    return left.number > right.number;
  }
};

int main(void) {

  srand(time(NULL));

  int K, D, V, N;

  cout << "Enter Topic number : Document number" << endl;
  cin >> K >> D;

  vector < string > W, Vocabulary;
  // number of words from topic t and document D,

  vector < vector < int > > n_D_t, n_t_V; // word count per document per topic, word count per topic (within corpus) , n_t_W can be obtained from n_t_V
  vector < int > total_NDT, total_NTV;

  vector < int > Z; //topic assignment
  vector < double > P;
  P.resize(K);

  n_D_t.resize(D);
  n_t_V.resize(K);

  double eta, alpha;

  alpha = 50.0 / K * 1.0;
  eta = 0.1;

  for (int i = 0; i < D; i++) {

    ifstream myfile;

    stringstream data;
    data << i + 1;
    string file_name = data.str();

    myfile.open(file_name);

    if (!myfile.is_open()) {
      cout << "File could not be open." << endl;

    }

    string file_data;

    while (myfile >> file_data) {

      Document_collection[i].push_back(file_data);
      // initialize W array
      W.push_back(file_data);

      //cout<<file_data<<endl;

    }

    myfile.close();
  }

  set < string > vocabSet(W.begin(), W.end());

  Vocabulary.assign(vocabSet.begin(), vocabSet.end());

  cout << "Vocabulary: " << endl;
  for (int i = 0; i < Vocabulary.size(); i++)
    cout << Vocabulary[i] << endl;

  V = Vocabulary.size();
  N = W.size();
  Z.resize(N);

  // initializing n_D_t
  for (int i = 0; i < D; i++)
    n_D_t[i].resize(K);

  //initializing n_t_V
  for (int i = 0; i < K; i++)
    n_t_V[i].resize(V);

  // initialize Z

  //   cout<<"INITIALIZE Z:"<<endl;

  for (int i = 0; i < N; i++) {
    Z[i] = rand() % K;
    //  cout<<rand() % K <<endl;
    // P[i] = 0;
  }

  for (int i = 0; i < K; i++)
    P[i] = 0;

  int max_iteration = (10000);

  vector < int > denominator_sum;

  total_NDT.resize(D);
  total_NTV.resize(K);

  int doc_no[N] = {
    0
  };

  // total number of words in corpus
  // initialize n_D_t, n_t_V,  P

  //cout<<"Y HELLO WORLD"<<endl;

  // initializing n_D_t

  for (int i = 0; i < D; i++)
    for (int j = 0; j < K; j++)
      n_D_t[i][j] = 0;

  for (int i = 0; i < K; i++)
    for (int j = 0; j < V; j++)
      n_t_V[i][j] = 0;

  for (int i = 0; i < D; i++)
    total_NDT[i] = 0;

  for (int i = 0; i < K; i++)
    total_NTV[i] = 0;

  for (int i = 0; i < N; i++) {

    int doc_no = find_document(i);
    int topic = Z[i];
    //            cout<<"Doc no: "<<doc_no<<" "<<topic<<endl;

    n_D_t[doc_no][topic]++;
  }

  for (int i = 0; i < D; i++) {
    for (int x = 0; x < K; x++) {

      total_NDT[i] += n_D_t[i][x];

    }

  }

  //cout<<"X HELLO WORLD"<<endl;

  //initializing n_t_V

  cout << "Random Topic assignment: " << endl;

  for (int i = 0; i < N; i++)
    cout << Z[i] << "  ";
  cout << endl << endl;

  for (int i = 0; i < K; i++) {

    int topic = i;

    for (int j = 0; j < V; j++) {

      string vocab = Vocabulary[j];

      for (int k = 0; k < N; k++) {

        if (W[k] == vocab && Z[k] == topic)
          n_t_V[topic][j]++;
      }

    }

  }

  //total calculation

  for (int i = 0; i < K; i++) {
    for (int x = 0; x < V; x++) {

      total_NTV[i] += n_t_V[i][x];

    }

  }

  //  cout<<"AFTER INIT: HELLO WORLD: N="<<N<<endl;

  cout << "Topic distributions: BEFORE" << endl;

  for (int i = 0; i < n_t_V.size(); i++) {
    cout << "Topic " << i << ": " << endl;
    for (int j = 0; j < n_t_V[i].size(); j++) {

      cout << n_t_V[i][j] << " ";

    }
    cout << endl;
  }

  cout << endl << endl;

  for (int i = 0; i < N; i++)
    doc_no[i] = find_document(i);

  for (int r = 0; r < max_iteration; r++) {

    cout << r << endl;

    for (int i = 0; i < N; i++) {

      string word = W[i]; // determine from  'i' the document that W[i] belongs to
      int document_no = doc_no[i];

      int topic = Z[i];

      int vocab_index = -1;

      int size_vocabulary = Vocabulary.size();

      for (int a = 0; a < size_vocabulary; a++) {

        if (Vocabulary[a] == word) {
          vocab_index = a;
          break;
        }

      }

      //    cout<<"BEFORE NDT_"<<endl;
      n_D_t[document_no][topic] -= 1;
      n_t_V[topic][vocab_index] -= 1;

      total_NDT[document_no] -= 1;
      total_NTV[topic] -= 1;
      //      cout<<"AFTER NDT_"<<endl;

      //cout<<"WHAT'S UP BRO!"<<endl;

      for (int t = 0; t < K; t++) {

        //    cout<<"BEFORE: ntV"<<endl;
        int n_D_t_i = n_D_t[document_no][t];
        int n_t_W_i = n_t_V[t][vocab_index]; //- 1; // i'th instance of W is subtracted from it's n_t_W, n_t_V is a distribution , t er under sob word er e count ase

        P[t] = (alpha + n_D_t_i) * (eta + n_t_W_i) * 1.0;
        //        cout<<"IS IT ZERO????????????????????????????????????????????????????????????????????????????????????????"<<endl;
        P[t] /= (K * alpha * 1.0 + total_NDT[document_no]) * (V * eta * 1.0 + total_NTV[t]);

      }

      double total = 0;

      for (int h = 0; h < K; h++) {

        total += P[h];
      }

      for (int h = 0; h < K; h++) {

        P[h] /= total;
      }

      //   cout<<"HELLO?"<<endl;

      // cumulative vector generation

      vector < double > cumulative_weight;
      cumulative_weight.resize(P.size() + 1);
      cumulative_weight.push_back(0);
      int t_sample;

      int N_Size = cumulative_weight.size();

      for (int a = 0; a < P.size(); a++) {

        cumulative_weight[a + 1] = cumulative_weight[a] + P[a];

      }

      // RESAMPLING

      double random_value = (rand() * 1.0 / RAND_MAX * 1.0);

      for (int a = 0; a < N_Size - 1; a++) {

        if (random_value < cumulative_weight[a + 1]) {
          t_sample = a;
          break;

        }
      }

      // normalize P
      // sample from P

      //  cout<<"BEFORE: T sample: "<<t_sample<<" document_no: "<<document_no<<" vocab_index"<<vocab_index<<endl;

      Z[i] = t_sample;
      n_D_t[document_no][t_sample] += 1;
      n_t_V[t_sample][vocab_index] += 1;
      total_NDT[document_no] += 1;
      total_NTV[t_sample] += 1;

      //    cout<<"AFTER: T sample: "<<t_sample<<" document_no: "<<document_no<<" vocab_index"<<vocab_index<<endl;

    }

    //  cout<<"I DONE"<<endl;

  }

  cout << "ITERATIONS DONE" << endl;

  // display words
  int display_words = 3;

  if (display_words > V)
    cout << "Can't show more words than the vocabulary size, invalid input." << endl;

  vector < vector < string > > showTopic;

  showTopic.resize(K);

  for (int i = 0; i < K; i++) {

    cout << "SORTING STARTED" << endl;

    vector < data > items;

    for (int k = 0; k < V; k++) {

      items.push_back(data(n_t_V[i][k], k));
    }
    cout << "INSERTION FAILED?" << endl;

    sort(items.begin(), items.end(), by_number());

    cout << "SORTING FAILED" << endl;

    cout << "After sorting: " << endl;
    for (int p = 0; p < items.size(); p++) {
      cout << items[p].index << " ";
    }

    cout << endl;

    for (int j = 0; j < display_words; j++) {

      int index = items[j].index;

      string word = Vocabulary[index];
      //            cout<<word<<endl;

      showTopic[i].push_back(word);

    }

    items.clear();
  }

  cout << "NOT REACHNIG" << endl;

  cout << "Topic distributions: " << endl;

  for (int i = 0; i < n_t_V.size(); i++) {
    cout << "Topic " << i << ": " << endl;
    for (int j = 0; j < n_t_V[i].size(); j++) {

      cout << n_t_V[i][j] << " ";

    }
    cout << endl;
  }

  cout << endl << endl;

  for (int i = 0; i < showTopic.size(); i++)
    cout << showTopic[i].size() << endl;

  cout << "After: Iterations" << endl;
  for (int i = 0; i < K; i++) {
    //        cout<<"Topic "<<i<<": "<<endl;
    for (int j = 0; j < display_words; j++) {
      cout << showTopic[i][j] << " ";
    }
    cout << endl << endl;
  }

  ofstream out_file("100KIteration.csv");

  //cout<<"After: Iterations"<<endl;
  for (int i = 0; i < K; i++) {
    //        cout<<"Topic "<<i<<": "<<endl;
    for (int j = 0; j < display_words; j++) {

      if (j != display_words - 1)
        out_file << showTopic[i][j] << ", ";
      else
        out_file << showTopic[i][j];
    }
    out_file << endl;
  }

  out_file.close();

  return 0;
}