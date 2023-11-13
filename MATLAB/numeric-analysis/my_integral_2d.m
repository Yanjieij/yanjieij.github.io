Alpha = 0:0.1:1;

syms x t p;
f = (t-x)^(p-1) * (-cos(x));
F = matlabFunction(f);

hold on;
legendInfo = cell(length(Alpha), 1);
for i=1:length(Alpha)
    p_val = Alpha(i);
    t_val = 0:0.1:3;
    F_val = arrayfun(@(t) integral(@(x) F(t, p_val, x), 0, t), t_val);
    disp(F_val);
    plot(t_val, F_val);
    xlabel('t');
    ylabel('F');
    legendInfo{i} = ['Alpha = ', num2str(Alpha(i))];
end
legend(legendInfo);
title('Results of integral');
hold off;
