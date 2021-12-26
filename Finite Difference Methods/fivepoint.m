clear
nx = 65; 
h = 1 / (nx - 1); 
%h = 1 / (nx - 1); 
diag_block = eye(nx - 1) * (-2 / h^2 - 2 / h^2); 
diag_block = diag_block + diag(ones(nx-2, 1) / h^2, 1); 
diag_block = diag_block + diag(ones(nx-2, 1) / h^2, -1); 
Matrix = kron(eye(nx - 1), diag_block); 
Matrix = Matrix + diag(ones((nx - 2) * (nx - 1), 1) / h^2, nx - 1);
Matrix = Matrix + diag(ones((nx-2) * (nx-1), 1) / h^2, -(nx-1)); 
x = [1:(nx-1)] * h; 
y = [1:(nx-1)] * h; 
% [X, Y] = meshgrid(x, y); 
[Y, X] = meshgrid(x, y);  

% the f(x) after applying the - discrete laplacian to the given function 
F = 4.*sin(X) .* cos(X) - 2; 
f = reshape(F', (nx-1) * (nx - 1), 1); 
surf(X, Y, F); 
xlabel('X')
ylabel('Y')
u = Matrix \ f; 
U = reshape(u, nx-1, nx-1)'; 
surf(X, Y, U)
xlabel('X')
ylabel('Y')
G = (sin(X) .* cos(X) + Y .* Y) .* h^2; 
g = reshape(G', (nx-1)*(nx-1), 1); 
v = Matrix \ (f + g); 
V  = reshape(v, nx-1, nx-1)';
surf(X, Y, V)
xlabel('X')
ylabel('Y')
surf(X, Y, G)
xlabel('X')
ylabel('Y')
diff = abs(minus(U, V));
max_err = max(max(diff));
