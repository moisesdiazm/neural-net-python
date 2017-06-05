function [ u ] = run_forward( e, w, v, x)
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here
  
  
  for j = 1:3
    suma = 0;
    for i = 1:1
      suma = suma + w(j,i) * e;
    end
    x(j) = 2.0 / (1.0 + exp(-suma)) - 1.0;
  end


  suma = 0;
  for i = 1:3
    suma = suma + v(i) * x(i);
  end
  
  u = 2.0 / (1.0 + exp(-suma)) - 1.0;

end

