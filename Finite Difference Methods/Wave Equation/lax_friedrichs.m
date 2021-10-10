function [u,u_exact,x,t] = lax_friedrichs(f,t_0,t_f,time_step,spatial_step)

h = 2*pi/spatial_step;
x = 0:h:2*pi;
x = x';

k = (t_f-t_0)/time_step;
t = t_0:k:t_f;

CLF = k/h;

u = zeros(spatial_step+1,time_step+1);
u(:,1) = f(x);
u_exact(:,1) = u(:,1);

for j=1:time_step
    for k=2:spatial_step
        u(k,j+1) = (u(k+1,j)+u(k-1,j))/2-(CLF/2)*(u(k+1,j)-u(k-1,j));
    end
    u(1,j+1)=1;
    u(spatial_step+1,j+1)=0;
    X = x-t(j+1);
    u_exact(:,j+1) = f(X);
end
