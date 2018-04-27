clear; clc; close all;

dirlist = glob("bs*.txt");

for i = 1:length(dirlist)
  filename = dirlist{i, 1};
  subject = load(filename);
  
  max_sub = max(subject);
  min_sub = min(subject);
  x = size(subject, 1);
  y = max_sub - min_sub;
  
  #coefs = vertcat(coefs, atan2(y,x));
  