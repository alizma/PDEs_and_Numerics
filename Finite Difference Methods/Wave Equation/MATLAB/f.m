function y = f(x)

x1 = x(x >= 0 & x <= pi); 
y(x >= 0 & x <= pi) = x1; 

x2 = x(x > pi & x < 2 * pi); 
y(x > pi & x < 2 * pi) = 2*pi - x2; 

y(x<0) = 0; 
y(x >= 2 * pi) = 0; 
