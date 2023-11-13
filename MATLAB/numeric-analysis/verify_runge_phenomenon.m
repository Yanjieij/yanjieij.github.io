lagrange_interpolation_and_plot(5);
lagrange_interpolation_and_plot(8);
lagrange_interpolation_and_plot(12);
lagrange_interpolation_and_plot(20);

function C = lagrange_interpolation_and_plot(num)
    X = linspace(-1, 1, num); 
    Y = 1 ./ (1 + 12 * X .^ 2);
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

    figure;
    x_dense = linspace(-1, 1, 1000);
    y_dense = polyval(C, x_dense);
    %plot(X, Y, 'ro'); 
    hold on;
    plot(x_dense, y_dense, 'b-');

    x = linspace(-1, 1, 1000);
    y = 1 ./ (1 + 12 * x .^ 2);
    plot(x, y, 'r-');
    
    y_point = polyval(C, 0.98);
    disp(y_point);
    plot(0.98, y_point, 'ro');
    text(0.98, y_point, ' x=0.98', 'VerticalAlignment','bottom', 'HorizontalAlignment','right')

    title(strcat(num2str(num-1),'-th Lagrange Interpolation and 1/(1+12*x^2)'));
    xlabel('x');
    ylabel('y');
    legend('Approximation Results', '1/(1+12*x^2)', 'Point of 0.98');
    hold off;
end
