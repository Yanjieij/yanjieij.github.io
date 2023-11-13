X = 0:1:24;
Y = [14,13,13,13,13,14,15,17,19,21,22,24,27,30,31,30,28,26,24,23,21,19,17,16,14];

n=length(X);
F=zeros(n,4);
for k=1:4
    F(:,k)=X'.^(k-1);
end
A=F'*F;
B=F'*Y';
C=A\B;
C3=flipud(C);
disp(C3);

n=length(X);
F=zeros(n,6);
for k=1:6
    F(:,k)=X'.^(k-1);
end
A=F'*F;
B=F'*Y';
C=A\B;
C5=flipud(C);
disp(C5);

x_dense = linspace(0, 24, 1000);
y_dense = polyval(C3, x_dense);
hold on;
plot(x_dense, y_dense, 'r-');
y_dense = polyval(C5, x_dense);
plot(x_dense, y_dense, 'b-');
plot(X,Y,'go');
title("LSM Fitting");
xlabel('x');
ylabel('y');
legend('3rd Fitting Results','5ed Fitting Results','Points');    
