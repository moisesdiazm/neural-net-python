function [ w,v ] = inicializacion( w, v )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
%% Seccion 1

for i = 1:3
    v(i) = random('unif',-1,1);
end

for i = 1:3
    for j = 1:1
        w(i,j) = random('unif',-1,1);
    end
end


end

