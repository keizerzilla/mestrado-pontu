Table t1;
Table t2;
ArrayList<Float> c1 = new ArrayList<Float>();
ArrayList<Float> c2;

void setup() {
  size(800, 600, P3D);
  
  t1 = loadTable("../../dump/bunny.xyz", "header, csv");
  for (TableRow row : t1.rows()) {
    float x = 1000*row.getFloat("x");
    float y = 1000*row.getFloat("y");
    float z = 1000*row.getFloat("z");
    
    c1.add(1);
  }
  
  t2 = loadTable("../../dump/bunny.xyz", "header, csv");
}

void draw() {
  background(51);
  stroke(255);
  strokeWeight(4);
  translate(width/2, height/2);
  for (TableRow row : t1.rows()) {
    float x = 1000*row.getFloat("x");
    float y = 1000*row.getFloat("y");
    float z = 1000*row.getFloat("z");
    point(x, y, z);
  }
}
