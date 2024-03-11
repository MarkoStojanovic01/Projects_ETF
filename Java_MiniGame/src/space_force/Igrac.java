package space_force;

import java.awt.Color;
import java.awt.Graphics;

public class Igrac extends Nebesko_telo {
	
	private int x_s[] = new int[3];
	private int y_s[] = new int[3];

	public Igrac(int x,int y) {
		super(x,y,Color.RED,10);
		trougao();
	}
	
	public int vrhX() {
		return x;
	}
	
	public int vrhY() {
		return y - r;
	}
	
	@Override
	public void setX(int x) {
		this.x += x;
		for(int i=0;i<3;i++)
			x_s[i] += x;
	}
	
	private void trougao() {
		double angle = 3*Math.PI/2;
		for (int i=0; i<3; i++) {
			x_s[i] = x + (int)(this.getR() * Math.cos(angle));
			y_s[i] = y + (int)(this.getR() * Math.sin(angle));
			angle += 2*Math.PI/3;
		}
	}
	
	@Override
	public void paint(Graphics g) {
		Color prevColor = g.getColor();
		g.setColor(boja);
		g.fillPolygon(x_s,y_s,3);
		g.setColor(prevColor);

	}

	@Override
	public boolean sadrziTacku(int i, int j) {
		return false;
	}

}
