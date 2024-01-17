import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';

@Component({
  selector: 'app-login-manager',
  templateUrl: './login-manager.component.html',
  styleUrls: ['./login-manager.component.css']
})
export class LoginManagerComponent implements OnInit {

  constructor(private router: Router, private userService: UserService) { }

  username: string = "";
  password: string = "";
  error: string;

  ngOnInit(): void {
  }

  login() {
    if (this.username == "" || this.password == "") {
      this.error = "Please enter both username and password!";
      return;
    }
    this.error = "";
    this.userService.login(this.username, this.password).subscribe((u: User) => {
      if (u && u.type == 'manager') {
        localStorage.setItem("user", u.username)
        localStorage.setItem("type", u.type)
        this.router.navigate([u.type])
      } else {
        this.error = "Incorrect input!";
        return;
      }
    })

  }

}
