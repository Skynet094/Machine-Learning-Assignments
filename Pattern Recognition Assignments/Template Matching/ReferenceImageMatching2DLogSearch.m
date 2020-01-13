
% template1 = double(imread('reference.jpg'));
% input = VideoReader('input.MOV');
%  testImage1 = read(input,100);
% 
%  template = template1(:,:,1);
%  testImage = testImage1(:,:,1);

template=[4 2; 3 5];
testImage=[3 4 4 2 6; 3 5 4 2 7; 8 7 4 5 7; 3 5 4 2 7; 5 6 3 5 9; 3 4 5 6 7];
[M, N]=size(template);
[I, J]=size(testImage);

p=7;

k = ceil(log10(p)/log10(2)); 

stepSize = 2^(k-1);

minCost=100000.0;
m1=0;
n1=0;

for i = 1: 2*p : I-M
    for j = 1: 2*p : J-N
        centerX= i+p-1; 
        centerY = j+p-1;
       
        %fprintf('%d %d\n',centerX,centerY);
        
         
        %cost for center
%         cost =0.0;
%         for x=centerX: centerX+M-1
%              for y= centerY: centerY+N-1
%                  if (x<1 || x>I || y <1 || y>J)
%                      continue
%                  end
%                   cost= cost + abs(template(x-centerX+1,y-centerY+1)-testImage(x,y));
%              end
%         end
%         
       
        % search the [-p, p] X [-p, p] window centered at (m,n) 
        while(stepSize>=1)
            for x = -stepSize : stepSize : stepSize        
                 for y = -stepSize : stepSize : stepSize
                     
                     % fprintf('%d %d\n',x,y);
                       m = x + centerX;
                       n = y + centerY;
                       if(m<1 || m>I || n<1 || n>J)
                           continue;
                       end
                     
                      cost = 0.0;
                      for k = m : m+M-1
                          for l = n: n+N-1
                              cost = cost + abs(template(k-m+1,l-n+1)-testImage(k,l));
                          end
                      end
                      
                      if(minCost>cost)
                           minCost=cost;
                           m1= m;
                           n1= n;    
                      end
                      

                      
                 end
            end

            stepSize = stepSize/2;
            centerX =  m1;
            centerY =  n1;     
        end
        
    end
end
   
%img1 = insertShape(testImage1,'Rectangle',[n1 m1 M N]);
%imshow(img1);    
fprintf('%f %d %d\n',minCost,m1,n1);
    
       
        

