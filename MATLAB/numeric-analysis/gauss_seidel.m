A = [4,-1,0,0,0;
    -1,4,-1,0,0;
    0,-1,4,-1,0;
    0,0,-1,4,-1;
    0,0,0,-1,4];

B = [100,0,0,0,200];

P = [0,0,0,0,0];
%P = [27.0513,8.20513,5.76923,14.8718,53.7179];

X = [0,0,0,0,0];

delta = 1e-6;
max1 = 100;

N = length(B);
for k=1:max1
    for j=1:N
        if j==1
            X(1)=(B(1)-A(1,2:N)*P(2:N)')/A(1,1);
        elseif j==N
            X(N)=(B(N)-A(N,1:N-1)*X(1:N-1)')/A(N,N);
        else
            X(j)=(B(j)-A(j,1:j-1)*X(1:j-1)'-A(j,j+1:N)*P(j+1:N)')/A(j,j);
        end
    end
    err=abs(norm(X-P));
    relerr=err/(norm(X)+eps);
    P=X;
    if (err<delta)||(relerr<delta)
        break
    end
end
X=X';

for i = 1:length(X)
    fprintf('%.6f\n', X(i))
end

