Alpha = 0:0.1:1;
t_val = 0:0.1:3;

F_real = zeros(length(t_val), length(Alpha));
F_imag = zeros(length(t_val), length(Alpha));

syms x t p;
f = (t-x)^(p-1) * (-cos(x));
F = matlabFunction(f);

for i=1:length(Alpha)
    p_val = Alpha(i);
    for j=1:length(t_val)
        F_val = integral(@(x) F(t_val(j), p_val, x), 0, t_val(j));
        
        F_real(j, i) = real(F_val);
        F_imag(j, i) = imag(F_val);
    end
end

[Alpha_grid, t_grid] = meshgrid(Alpha, t_val);

figure;
surf(Alpha_grid, t_grid, F_real);
title('Real part of F');
xlabel('Alpha');
ylabel('t');
zlabel('Real part of F');

figure;
surf(Alpha_grid, t_grid, F_imag);
title('Imaginary part of F');
xlabel('Alpha');
ylabel('t');
zlabel('Imaginary part of F');
