import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';

@Component({
  selector: 'app-password',
  templateUrl: './password.component.html',
  styleUrls: ['./password.component.css']
})
export class PasswordComponent implements OnInit {

  constructor(private router: Router, private userService: UserService) { }

  user: User = null;
  returnAdress: string = '';

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient' && type != 'doctor') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getUser(username).subscribe((u: User) => {
      this.user = u;
    })
    this.returnAdress = localStorage.getItem("return");
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  goBack(){
    this.router.navigate([this.returnAdress]);
  }

  message: string;
  password1: string;
  password2: string;
  newPassword: string;

  isPass1Valid = true;
  isPass2Valid = true;
  isNewPassValid = true;
  oldPass = true;
  samePass = true;
  passForm = true;


  changePass(){

    this.isPass1Valid = !!this.password1;
    this.isPass2Valid = !!this.password2;
    this.isNewPassValid = !!this.newPassword;
    this.oldPass = true;
    if(this.user.password != this.password1 && this.isPass1Valid) this.oldPass = false;
    this.samePass = true;
    this.passForm = true;
    if(this.isNewPassValid){
      this.passForm = this.validatePassword(this.newPassword);
    }
    if(this.newPassword != this.password2 && this.isNewPassValid && this.isPass2Valid){
      this.samePass = false;
    }

    if(this.isPass1Valid && this.isPass2Valid && this.isNewPassValid &&
      this.oldPass && this.samePass && this.passForm){

        this.userService.updatePassword(this.user.username,this.newPassword).subscribe(respObj=>{
          if(respObj['message']=='ok'){
            this.message = 'Successfully updated!'
            alert(this.message)
            this.logout();
          }
          else{
            this.message = 'Error'
            alert(this.message) 
          }
        });
      }
    else{
      alert('Please fix the form errors before submitting.');
    }
  }

  validatePassword(password: string): boolean {
    const minLength = 8;
    const maxLength = 14;
    
    if (password.length < minLength || password.length > maxLength) {
      return false;
    }
  
    const regex = /^(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*])[A-Za-z][A-Za-z0-9!@#$%^&*]{6,12}$/;
  
    if (!regex.test(password)) {
      return false;
    }
  
    for (let i = 0; i < password.length - 1; i++) {
      if (password[i] === password[i+1]) {
        return false;
      }
    }
  
    return true;
  }

}
