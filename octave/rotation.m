function [rx, ry, rz] = rotation(theta)
    rx = [[0            0           1],
          [0 cosd(theta) -sind(theta)],
          [0 sind(theta)  cosd(theta)]];
    
    ry = [[cosd(theta)  0 sind(theta)],
          [0            1           0],
          [-sind(theta) 0 cosd(theta)]];
    
    rz = [[cosd(theta) -sind(theta) 0],
          [sind(theta)  cosd(theta) 0],
          [0            0           1]];
endfunction

