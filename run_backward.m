function [ w,v ] = run_backward( e, error, u, w, v, x)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

  ETA = .01;
  
  delta = error * u * (1 - u);
  for i = 1:1
      v(i) = v(i) + ETA * delta * x(i) * 1.; 
  end

  for j = 1:3
    delta2 = delta * v(j) * x(j) * (1 - x(j)); 
    for i = 1:1
      w(j,i) = w(j,i) + ETA * delta2 * e * 1.;
    end
  end

end

