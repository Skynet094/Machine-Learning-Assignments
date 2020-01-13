
template1 = double(imread('reference.jpg'));
input = VideoReader('input.MOV');
testImage1 = double(read(input,1));
template = template1(:,:,1); %only red component
testImage= testImage1(:,:,1);

%imshow(template1);
%template=[4 2; 3 5];
%testImage=[3 4 4 2; 3 5 4 2; 8 7 3 5; 3 5 4 2];
[M,N]=size(template);

[I,J]=size(testImage);

C=1000000.0;
    m1 = 0;
    n1 = 0;
    for m = 1: I-M
       for n = 1 : J-N
           cost = 0.0;
           for i=m : m+M-1
               for j =n : n+N-1
                   
                    cost = cost + abs(template(i-m+1,j-n+1)-testImage(i,j));
               end
           end
           if C > cost
               C = cost;
               m1 = m;
               n1 = n;
               %fprintf('%f %d %d\n',C,m1,n1);
           end
        
        end
    end
    image= insertShape(testImage1,'Rectangle',[n1 m1 M N]);
    imshow(image);
    fprintf('%f %d %d\n',C,m1,n1);
    for i=m1: m1+M-1
        for j=n1: n1+N-1
            fprintf('%d ',testImage(i,j));
        end
        fprintf('\n');
    end