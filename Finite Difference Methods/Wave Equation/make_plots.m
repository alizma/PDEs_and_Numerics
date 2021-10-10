clear

t_0 = 0; t_f = 1; spatial_steps = 2^7; time_steps = 2^8;
[u,u_exact,x,t] = explicit_upwind(@f,t_0,t_f,time_steps,spatial_steps);
figure(1)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.1,4.5])
title('Upwind Scheme','FontSize',16)
print -dps explicit_upwind.ps
figure(1)

[u,u_exact,x,t] = lax_friedrichs(@f,t_0,t_f,time_steps,spatial_steps);
figure(2)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.1,4.5])
title('Lax-Friedrichs Scheme','FontSize',16);
print -dps lax_friedrichs.ps
figure(2)

[u,u_exact,x,t] = lax_wendroff(@f,t_0,t_f,time_steps,spatial_steps);
figure(3)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.3,4.5])
title('Lax-Wendroff Scheme','FontSize',16)
print -dps lax_wendroff.ps
figure(3)

[u,u_exact,x,t] = centered_difference(@f,t_0,t_f,time_steps,spatial_steps);
figure(4)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.3,4.5])
title('Naive Centered Difference Scheme','FontSize',16)
print -dps centered_difference.ps
figure(4)

[u,u_exact,x,t] = backward_euler(@f,t_0,t_f,time_steps,spatial_steps);
figure(5)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.3,4.5])
title('Backward Euler Scheme','FontSize',16)
print -dps centered_difference.ps
figure(5)

[u,u_exact,x,t] = crank_nicolson(@f,t_0,t_f,time_steps,spatial_steps);
figure(6)
clf
plot(x,u(:,101),'-','LineWidth',2);
hold on
plot(x,u_exact(:,101),'--','LineWidth',2)
hold off
axis([0,2*pi,-0.3,4.5])
title('Crank-Nicolson Scheme','FontSize',16)
print -dps centered_difference.ps
figure(6)

