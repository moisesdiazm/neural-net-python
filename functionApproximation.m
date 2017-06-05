clear all
close all

w = zeros(3,1);
v = zeros(3,1); 
x = zeros(3,1);

e = 0; % referencia
suma = 0;
u = zeros(100,1);
ref = zeros(100,1);

error = 0.0;

[w,v] = inicializacion(w,v);

%% otra cosa
for f = 1:100
    for a = 1:100
        u(a) = run_forward(a/100, w, v, x);

        % calcular error
        ref(a) = rect(a);
        error = ref(a) - u(a);

        [w,v] = run_backward(a/100, error, u(a), w, v, x);
    end
end

plot(u)
hold on
plot(ref)