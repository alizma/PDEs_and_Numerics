function c = fdcoeffV(k, xbar, x) 
A = ones(n, n); 
xrow = (x(:) - xbar)'; 
for i = 2:n 
    A(i, :) = (xrow .^ (i-1)) ./ factorial(i-1); 
end 

b = zeros(n,1); 
b(k+1) = 1; 
c = A\b; 
c = c'; 
% if u is the column vector of n values u(xi), then the resulting approximation to u^(k) (xbar) = c*u