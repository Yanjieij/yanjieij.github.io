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
        X(j)=(B(j)-sum(A(j,[1:j-1,j+1:N]).*P([1:j-1,j+1:N])))/A(j,j);
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
