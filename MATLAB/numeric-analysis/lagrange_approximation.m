x = 0:0.01:1*pi;
y = cos(x);
figure;
plot(x,y)
title('Plot for cos(x)');

X1 = [0,1/2*pi,3/4*pi];
Y1 = [cos(0),cos(1/2*pi),cos(3/4*pi)];
C1 = lagrange_interpolation(X1,Y1);
figure;
plot_lagrange(X1, Y1, C1);
title('Second-order Lagrange Interpolation for cos(x)');

X2 = [0,1/3*pi,2/3*pi,1*pi];
Y2 = [cos(0),cos(1/3*pi),cos(2/3*pi),cos(1*pi)];
C2 = lagrange_interpolation(X2,Y2);
figure;
plot_lagrange(X2, Y2, C2);
title('Third-order Lagrange Interpolation for cos(x)');

X3 = [0,1/5*pi,2/5*pi,3/5*pi,4/5*pi,1*pi];
Y3 = [cos(0),cos(1/5*pi),cos(2/5*pi),cos(3/5*pi),cos(4/5*pi),cos(1*pi)];
C3 = lagrange_interpolation(X3,Y3); 
figure;
plot_lagrange(X3, Y3, C3);
title('Fifth-order Lagrange Interpolation for cos(x)');

fprintf('Drawing Complete.')

function plot_lagrange(X,Y,C)
    x_dense = linspace(0, pi, 1000);
    y_dense = polyval(C, x_dense);
    plot(X, Y, 'ro'); 
    hold on;
    plot(x_dense, y_dense, 'b-');
    title('Lagrange Interpolation');
    xlabel('x');
    ylabel('y');
    legend('Data points', 'Approxinmation Results');
    hold off;
end

function C = lagrange_interpolation(X, Y)
    w = length(X);
    n = w - 1;
    L = zeros(w, w);
    for k = 1: n + 1
        V = 1;
        for j = 1: n + 1
            if k ~= j
                V = conv(V, poly(X(j))) / (X(k) - X(j));
            end
        end
        L(k, :) = V;
    end
    C = Y * L;
end
