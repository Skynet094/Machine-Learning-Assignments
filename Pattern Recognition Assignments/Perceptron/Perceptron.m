


data_train = dlmread('trainNN.txt');

% count the number of features
% count number of examples

[numberOfSamples, numberOfFeatures] = size(data_train);
 % one column for training class

 for i=2: numberOfSamples
     for j=1: numberOfFeatures-1
         
         data_train(i,j) = data_train(i,j); % / max(data_train(:,j));
         
     end
 end
 

data_samples = ones(numberOfSamples-1, numberOfFeatures);

for i=2:numberOfSamples
    for j=1:numberOfFeatures-1
        data_samples(i-1,j+1)= data_train(i,j);
    end
end


% count number of classes 

c = data_train(:,numberOfFeatures);
class_labels= unique(c);



 [numberOfClasses, ~] = size(class_labels);

 % number of neurons in the hidden layers 
 
 layer_configuration = dlmread('layer_configuration.txt');

 % total number of layers
 
 [~, numberOfHiddenLayers] = size(layer_configuration);
 numberOfLayers= numberOfHiddenLayers + 1;
 % find unique number of classes and create class vectors 

 % so number of neurons in last layer == numberOfClasses
 
 % number of neurons in the first layer == numberOfFeatures
 
 y_class = zeros(numberOfClasses, numberOfClasses+1);
 
 
 for i= 1 : numberOfClasses
     y_class(i,1)=1;
     y_class(i,i+1) = 1;
 end
 
 % display(y_class);
 
 test_data_type = class_labels(1);
 
 if( isa(test_data_type, 'string') )
     data_type='string';
 else
     data_type ='uint32';
 end
 
 
 y_real = containers.Map('keyType',data_type,'valueType','Any');
 
 
 for i=1: numberOfClasses
     y_real( class_labels(i)) = y_class(i,:);
 end

% keys(y_real);
 values(y_real);
 
 % proti layer e koyta kore neuron ta to jani right? , 
 % first e layer size er ekta weight vector ber kori 
 
 
 % weight_vectors = rand( numberOfLayers, 1);
 
 weight_vectors = containers.Map('keyType','uint32','valueType','Any');
 
 weight_vector_matrix = containers.Map('keyType','uint32','valueType','Any');
 
 % first layer er kahini 
 
 % first layer e  layer_configuration(1,1)  songkhok neurons thakbe, but
 % proti neuron er weight er dimension hobe, input er dimension er soman
 
 value = layer_configuration(1,1);
 
 % value songkhok layer-1 neurons 
 
 for i=1 : value
     weight_vectors(i) = rand(1, numberOfFeatures);
%     fprintf('HELLO');
 %    display(weight_vectors(i));
 end
 
 weight_vector_matrix(1) = weight_vectors; 
 
 for i=1: numberOfHiddenLayers -1 % first layer ar last layer bade
     
     prev_layer= layer_configuration(1, i); % previous layer er size, porer layer 
  
     current_layer= layer_configuration(1, i+1);
     
    weight_vectors = containers.Map('keyType','uint32','valueType','Any');
   % fprintf('Current Layer: %d , Current layer neuron number: %d\n',i, current_layer);
   
   for j= 1 : current_layer
         weight= rand(1, prev_layer + 1);
         weight_vectors(j) = weight;     
         
    end
      weight_vector_matrix(i+1) = weight_vectors;     
 end
 
 
 % number of neurons in last layer == number Of Classes 
 value = layer_configuration(1, numberOfLayers -1 );
 weight_vectors = containers.Map('keyType','uint32','valueType','Any');
    
 for i=1: numberOfClasses
    weight_vectors(i) = rand(1, value + 1); 
 end
 
weight_vector_matrix(numberOfLayers) = weight_vectors;

 % WEIGHT MATRIX HAS WEIGHTS FOR ALL NEURONS WITH RESPECT TO LAYERS 
 
% for i= 1: numberOfLayers
 %    display(weight_vectors(i))
 %end
 
 
 Y_r = containers.Map('keyType','uint32','valueType','Any');
 V_r = containers.Map('keyType','uint32','valueType','Any');
 
 eta_r = containers.Map('keyType','uint32','valueType','Any');
 
 Y_r(1) = rand(1, numberOfFeatures);
 
 y= Y_r(1); 
 y(1,1) = 1;
 Y_r(1) = y;
 
 for i=1: numberOfHiddenLayers 
     Y_r(i+1) = rand(1 , layer_configuration(i) + 1) ;
      
    y= Y_r(i+1); 
    y(1,1) = 1;
    Y_r(i+1) = y;    
     V_r(i) = rand(1 , layer_configuration(i)) ;
     eta_r(i) = rand(1, layer_configuration(i));
     
 end
 
 
 
 Y_r(numberOfLayers+1) = rand(1, numberOfClasses);  % here, y0 is not possible , so y1 is the new y0

 
 V_r(numberOfLayers) = rand(1, numberOfClasses);
 eta_r(numberOfLayers) = rand(1, numberOfClasses);
 
 % number of neurons in the last layer == number of classes  
 
 Y_r ( numberOfLayers + 1) = rand(1, numberOfClasses);
 
 learning_rate = 0.05;
 
 max_iteration = 200;
 
 alpha =1;
 
 % r0 calculation is done here
 
 
 % calculate Vr(i) , Yr(i,j)
 
 
% f(x) = 1/(1+ exp(-ax))
 
layer_information = rand(1, numberOfLayers);

layer_information(1, numberOfLayers) = numberOfClasses;


for k=1: numberOfHiddenLayers
    layer_information(1, k) = layer_configuration(1,k);
end


for ITER= 1 : max_iteration
     
  %  fprintf('NUMBER OF ITERATION: %d\n', ITER);
     for i = 1: 2%numberOfSamples - 1 
         
         X= data_samples(i,:);
         Y_r(1) = X;
       
    % Basically,  Y(r) theke , Y(r+1) construct kortesi 
    
     for r = 1 : numberOfLayers 

            numberOfNeurons= layer_information(1, r);
            
            weight_vectors= weight_vector_matrix(r);

            y_old = Y_r(r);
            y_new = Y_r(r+1);
            [~, test_size] = size(y_new);
            construct_y = zeros(1,test_size);
            construct_y(1,1) = 1;

            construct_v = zeros(1, numberOfNeurons);

            % FORWARD COMPUTATIONS 
            
            for j= 1: numberOfNeurons

                 % ei neuroner jonno kon weight, khuje ber koro
                 % r tomo layer er weight, from weight matrix

                 weight = weight_vectors(j); % r-th layer er j th neuron er weight

                  v_r = weight * transpose(y_old);  % different weight for different neuron, different v_r
                  
                  construct_v(1,j) = v_r; %  r tomo layer er j tomo neuron er v(r,j)
  
                  f_value = 1.0/ (1 + exp(-alpha * v_r));                                
                  construct_y(1, j+1) = f_value ;
                  
            end
            
             V_r(r) = construct_v;
             Y_r(r+1) = construct_y;

     end
         
     %    display(Y_r(2));
         
% FORWARD COMPUTATIONS END HERE

         % COMPUTATIONS FOR LAST LAYER
         % calculating the last layer stuff
             numberOfNeuronsLastLayer = layer_information(numberOfLayers) ;

             eta_last = eta_r(numberOfLayers);
             % last layer er , "eta" gula calculate kortesi
             y_cap = Y_r( numberOfLayers+1);
             v_L = V_r(numberOfLayers);

             
          for l=1 : numberOfNeuronsLastLayer
                 y_m = y_real(data_train(i+1, numberOfFeatures) );
               %  display(y_m);
                 y_expected = y_m(l);
                 y_predicted = y_cap(l);  % karon, first y0=1, y(l+1) = l'th neuron er output

                 v = v_L(l);
                 f_x = 1/(1+ exp(-alpha* v));
                 
  %               fprintf('l =%d EXPECTED = %f\n' ,l, y_expected);
                 
 %             fprintf('v=%f y predicted %f , y expected %f , eta= %f\n ' ,v,  y_predicted, y_expected, -( y_expected - y_predicted) * alpha * f_x * (1-f_x));
                 
                 
                 eta_last(1,l) =     -(y_expected - y_predicted) * alpha * f_x * (1-f_x)  ; 
  %              fprintf('y_delta = %f , f_x= %f, 1-f_x= %f\n' , -(y_expected- y_predicted) , f_x, 1-f_x);
          end
          
          
          
             eta_r(numberOfLayers) = eta_last;
%             display(eta_last);
         
             %fprintf('ETAAAA LASSSSSSSSSSSSST : %f\n', eta_last );
        
        % COMPUTATIONS FOR LAST LAYER END HERE
        
             % proti ta layer er jonno, proti ta neuron er jonno, delta(R,J)
             % ber korte hobe, ager layer er information use kore

       % BACKWARD CALCULATIONS START
       
             for R=1 : numberOfLayers -1 

                 numNeuronPrev = layer_information(numberOfLayers - R +1); %K 
                 eta_prev_layer = eta_r( numberOfLayers - R +1 ); 
                 w_vectors = weight_vector_matrix(numberOfLayers - R + 1);


                 numNeuronCurrent = layer_information( numberOfLayers - R);  % J
                 eta_current_layer = eta_r( numberOfLayers - R); 

                for J = 1: numNeuronCurrent

                     eta_sum= 0;

                     for K = 1: numNeuronPrev
                          weight_prev = w_vectors(K);
                          eta_sum  = eta_sum + eta_prev_layer(K)* weight_prev(J+1);
                     end

                     v_vect = V_r(numberOfLayers  - R );
                     v = v_vect(J);
                     f_x = 1/ (1+ exp(-alpha * v));
                     
                     eta = eta_sum * alpha * f_x * (1-f_x); 
                     eta_current_layer(1, J) = eta;
                     
                 end

                  eta_r(numberOfLayers - R) = eta_current_layer;
               %   display(eta_current_layer);
            end 
        % BACKWARD CALCULATION END HERE
        
             % sob eta calculation shesh, sob y_(r-1) calculate kora shesh

             % UPDATE WEIGHT HERE 

             for N=1: numberOfLayers

                 numberOfNeurons = layer_information(N); 
                 weightVectorForLayer= weight_vector_matrix(N); 

                y_r = Y_r(N); 
                eta_R = eta_r(N); 

                 for M=1: numberOfNeurons
                     eta_J = eta_R(M);
              %       fprintf('eta  = %f\n',eta_J);
                     weightVectorForLayer(M)= weightVectorForLayer(M) - learning_rate* abs(eta_J) * y_r ;
                  %   display(y_r * eta_J);
                 end
                 weight_vector_matrix(N) = weightVectorForLayer;

             end


     end
     
   %  fprintf('ITER = %d\n', ITER);
end











% TESTING PHASE START 

test_samples = dlmread('testNN.txt');

[ numberOfTestSamples , numberOfFeatures ] = size(test_samples);

 test_data = zeros( numberOfTestSamples, numberOfFeatures); 

 for i=1: numberOfTestSamples
    
     for j=1: numberOfFeatures
        if(j==1) 
             test_data(i,j)=1;
        else
            test_data(i,j) = test_samples(i,j-1); % /max(test_samples(:, j-1));
        end
     end
 end
 

 misclassification = 0;
 correctClassification=0;
 
for i = 1 :  numberOfTestSamples % insert number of samples -1

         X= test_data(i,:);
         Y_r(1) = X; 
   %      display(Y_r(1));
         
         next_R=0;

    % Basically,  Y(r) theke , Y(r+1) learn kortesi 
    
         for r = 1 : numberOfLayers 

             numberOfNeurons= layer_information(1, r );

            weight_vectors= weight_vector_matrix(r );

            y_prev = Y_r(r);
      
            y_new = Y_r(r+1);
            [~, test_size] = size(y_new);
        %    fprintf('test_size = %f\n', test_size);
            
            construct_y = zeros(1,test_size);
            construct_y(1,1) = 1;

            construct_v = zeros(1, numberOfNeurons);

            % FORWARD COMPUTATIONS 
            
            for j= 1: numberOfNeurons

      
                 weight = weight_vectors(j); % r-th layer er j th neuron er weight

                  v_r = weight * transpose(y_prev);  % different weight for different neuron, different v_r
                
                  
                  construct_v(1,j) = v_r; %  r tomo layer er j tomo neuron er v(r,j)
  
                  f_value = 1.0/ (1 + exp(-alpha * v_r))*1.0; 
                  construct_y(1, j+1) = f_value ;
                  
            end
            
             V_r(r) = construct_v;
             Y_r(r+1) = construct_y;

         end
         
       %  display(construct_y);
        % fprintf('What the heck is going on?\n');
     %    display(Y_r(2));
         
         
         % layer L er kahini 
         
           numberOfNeuronsLastLayer = layer_information(numberOfLayers) ;

             eta_last = eta_r(numberOfLayers);
             % last layer er , "eta" gula calculate kortesi
             y_cap = Y_r( numberOfLayers+1);
             
             y_test = zeros(0,numberOfNeuronsLastLayer);
             
             for k=2: numberOfNeuronsLastLayer+1
                 y_test(1, k-1) = y_cap(1,k);
             end
             
             max_value= max(y_test);
  %         display(y_cap);  
             for k=2: numberOfNeuronsLastLayer+1
                 if(y_cap(1,k)<max_value)
                     y_cap(1,k)=0;
                 else
                     y_cap(1,k)=1;
                 end
             end
             
          %    y_test = y_cap(1: 2,:);
             
             v_L = V_r(numberOfLayers);

         % y_predicted = (1:numberOfClasses);
          
       %   for l=1 : numberOfNeuronsLastLayer
         %     y_element_predict = 0;
        %  end
           
          y_m = y_real(test_samples(i, numberOfFeatures) );
          
     %     display(X);
   %       fprintf('Data sample: %d\n', i);
         % y_cap = y_cap / max_value; 
          
       %   display(y_cap);
 %        display(y_m);
          
                 
          if(y_cap == y_m)
        %       display(y_cap);
       %        display(y_m);
               correctClassification= correctClassification + 1;
          else
                misclassification = misclassification + 1;
           end
         
         %        fprintf('v=%f y predicted %f , y expected %f , eta= %f\n ' ,v,  y_predicted, y_expected, ( y_predicted - y_expected) * alpha * v * (1-v));
                 
                 
           
           
end


 fprintf('Correct classification: %f MissClassification: %f, testSamples= %d\n', correctClassification, misclassification, numberOfTestSamples);
 fprintf('Accuracy of the classifier= %f%\n', (correctClassification*100)/numberOfTestSamples);
 