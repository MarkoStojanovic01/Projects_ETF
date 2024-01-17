import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { ExaminationsService } from '../services/examinations.service';
import { UserService } from '../services/user.service';
import { User } from '../models/user';

@Component({
  selector: 'app-manager-user',
  templateUrl: './manager-user.component.html',
  styleUrls: ['./manager-user.component.css']
})
export class ManagerUserComponent implements OnInit {

  constructor(private route: ActivatedRoute, private router: Router,
    private userService: UserService, private examinationService: ExaminationsService) { }

  user: User;

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'manager') this.router.navigate(['']);
    let username = this.route.snapshot.paramMap.get('username');
    let manager = localStorage.getItem("user");
    manager = manager == null ? "" : manager; // check if username wasnt set
    if(manager == "") this.logout;
    this.userService.getUser(username).subscribe((u: User) => {
      if(u == null) this.router.navigate(['']);
      this.user = u;
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  deleteUser(){
    this.userService.deleteUser(this.user.username).subscribe(resp=>{
      alert(resp['message'])
      this.router.navigate(['manager'])
    })
  }

  updateUser(){
    if(this.user.type == 'patient'){
      localStorage.setItem('patient',this.user.username);
      this.route.url.subscribe(urlSegments => {
        const path = urlSegments.map(segment => segment.path).join('/');
        localStorage.setItem('return',path);
        this.router.navigate(['updatePatient'])
      });
    }
    else{
      localStorage.setItem('doctor',this.user.username);
      this.route.url.subscribe(urlSegments => {
        const path = urlSegments.map(segment => segment.path).join('/');
        localStorage.setItem('return',path);
        this.router.navigate(['updateDoctor'])
      });
    }
    
  }

}
