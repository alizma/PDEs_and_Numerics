function explicit_upwind(f,t_0,t_f, time_step,spatial_step)

    h = 2*pi/spatial_step
    x = 0:h:2*pi
    x = x'
    
    k = (t_f-t_0)/time_step
    t = t_0:k:t_f
    
    CLF = k/h
    
    u = zeros(spatial_step+1,time_step+1)
    u[:,1] = f[x]
    u_exact[:,1] = u[:,1]
    
    for j=1:time_step
        for k=2:spatial_step+1
            u[k,j+1] = u[k,j]-CLF*(u[k,j]-u[k-1,j])
        end
        u[1,j+1]=1
        X = x-t[j+1]
        u_exact[:,j+1] = f[X]
    end
    
    return [u,u_exact,x,t]
    end