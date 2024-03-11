package space_force;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Polygon;

public class Kometa extends Nebesko_telo {
	
	private int x_s[] = new int[5];
	private int y_s[] = new int[5];

	public Kometa(int x, int y, int r) {
		super(x, y, Color.GRAY, r);
		petougao();
	}
	
	private void petougao() {
		double angle = Math.random() * 2 * Math.PI;
		for (int i=0; i<5; i++) {
			x_s[i] = x + (int)(this.getR() * Math.cos(angle));
			y_s[i] = y + (int)(this.getR() * Math.sin(angle));
			angle += 2*Math.PI/5;
		}
	}
	
	@Override
	public void setY(int y) {
		this.y += y;
		for(int i=0;i<5;i++)
			y_s[i] += y;
	}

	@Override
	public void paint(Graphics g) {
		Color prevColor = g.getColor();
		g.setColor(boja);
		g.fillPolygon(x_s,y_s,5);
		g.setColor(prevColor);

	}

	@Override
	public boolean sadrziTacku(int i, int j) {
		if((new Polygon(x_s,y_s,5)).contains(i, j)) return true;
		return false;
	}
	
	

}
