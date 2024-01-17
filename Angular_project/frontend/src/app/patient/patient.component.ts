import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { UserService } from '../services/user.service';
import { User } from '../models/user';

@Component({
  selector: 'app-patient',
  templateUrl: './patient.component.html',
  styleUrls: ['./patient.component.css']
})
export class PatientComponent implements OnInit {

  constructor(private router: Router, private userService: UserService,
    private route: ActivatedRoute) { }

  user: User = null;

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getUser(username).subscribe((u: User) => {
      this.user = u;
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  updateUser(){
    localStorage.setItem('patient',this.user.username);
    this.route.url.subscribe(urlSegments => {
      const path = urlSegments.map(segment => segment.path).join('/');
      localStorage.setItem('return',path);
      this.router.navigate(['updatePatient'])
    });
    
  }

  changePass(){
    this.route.url.subscribe(urlSegments => {
      const path = urlSegments.map(segment => segment.path).join('/');
      localStorage.setItem('return',path);
      this.router.navigate(['password'])
    });
    
  }

}
