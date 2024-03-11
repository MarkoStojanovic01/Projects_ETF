package space_force;

import java.awt.BorderLayout;
import java.awt.Button;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Frame;
import java.awt.Label;
import java.awt.Panel;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;



public class Simulator extends Frame {

	private Panel bottomPanel = new Panel();
	private Panel topPanel = new Panel();
	//Panel centerPanel = new Panel();
	private Svemir svemir = new Svemir(this);
	private Generator generator = new Generator(svemir);
	private Igrac igrac;
	Label score = new Label();
	
	public Simulator() {
		setBounds(700,200,200,400);
		
		populateWindow();
		
		addWindowListener(new WindowAdapter() {
			public void windowClosing(WindowEvent e) {
				zavrsi();
			}
		});
		
		setVisible(true);
		setResizable(false);
	}

	private void populateWindow() {
		
		add(svemir, BorderLayout.CENTER);
		
		Button pokreni = new Button("Pokreni");
		
		pokreni.addActionListener((ae) -> {
			pokreni.setEnabled(false);
			svemir.setFocusable(true);
			svemir.requestFocus();
			svemir.postaviIgraca(igrac);
			generator.pokreni();
			svemir.pokreniSvemir();
		});
		
		bottomPanel.add(pokreni);
		add(bottomPanel, BorderLayout.SOUTH);
		
		topPanel.setBackground(Color.YELLOW);
		topPanel.add(new Label("Score: "));
		topPanel.add(score);
		
		add(topPanel,BorderLayout.NORTH);
		
		
		
		//System.out.println("pozicija:" + centerPanel.getWidth() + " " + centerPanel.getHeight());
		igrac = new Igrac(getWidth()/2, getHeight()-110);
		
		
		svemir.addKeyListener(new KeyAdapter() {
			@Override
			public void keyPressed(KeyEvent e) {
			char key = Character.toUpperCase(e.getKeyChar());
			switch(key) {
			case KeyEvent.VK_A:
			igrac.setX(-5); break;
			case KeyEvent.VK_D:
			igrac.setX(5); break;
			case KeyEvent.VK_W:
			svemir.dodajMetak(); break;
			}
			}
		});
		
		
		svemir.addMouseListener(new MouseAdapter(){

			public void mouseClicked(MouseEvent e) {
				svemir.dodajMetak();
			}
		});
		
		
		
		
		}
	
	public static void main(String[] args) {
		new Simulator();
	}
	
	public void gotovaIgra() {
		svemir.zavrsi();
		generator.zavrsi();
		topPanel.removeAll();
		topPanel.setBackground(Color.RED);
		topPanel.add(new Label("The end"));
		topPanel.revalidate();
	}
	
	public void zavrsi() {
		svemir.zavrsi();
		generator.zavrsi();
		dispose();
	}
		
	}

/*
 * svemir.addKeyListener(new KeyAdapter() {
@Override
public void keyPressed(KeyEvent e) {
char key = Character.toUpperCase(e.getKeyChar());
switch(key) {
case KeyEvent.VK_A:
igrac.setX(-5); break;
case KeyEvent.VK_D:
igrac.setX(5); break;
}
}

this.addMouseListener(new MouseAdapter() {
			
			public void mouseClicked(MouseEvent e) {
				plac plac = (plac)(getParent());
				plac.izaberiParcelu(parcela.this);
			}
		});
 */

