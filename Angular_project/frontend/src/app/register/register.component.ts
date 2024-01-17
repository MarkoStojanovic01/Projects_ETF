import { HttpClient } from '@angular/common/http';
import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';
import { Rejected } from '../models/rejected';

@Component({
  selector: 'app-register',
  templateUrl: './register.component.html',
  styleUrls: ['./register.component.css']
})
export class RegisterComponent implements OnInit {

  constructor(private router: Router, private userService: UserService,
     private http: HttpClient) { }

  users: User[] = [];
  rejected: Rejected[] = [];

  ngOnInit(): void {
    this.userService.getAllUsers().subscribe((data:User[])=>{
      this.users = data;
    })
    this.userService.getRejected().subscribe((data:Rejected[])=>{
      this.rejected = data;
    })
    
  }

  validateIntegerInput(event: any): void {
    if ((event.which < 48 || event.which > 57) && event.which !== 8) {
      event.preventDefault();
    }
  }

  message: string;
  username: string;
  password1: string;
  password2: string;
  firstname: string;
  lastname: string;
  adress: string;
  mail: string;
  phone: string;
  image: string;


  // Initialize boolean flags for input validation
  isUsernameValid = true;
  isPass1Valid = true;
  isPass2Valid = true;
  isFirstnameValid = true;
  isLastnameValid = true;
  isAdressValid = true;
  isMailValid = true;
  isPhoneValid = true;

  sameUsername = false;
  samePass = true;
  usedMail = false;
  passForm = true;
  rejectedUsername = false;
  rejectedMail = false;

  async addPatient(): Promise<void> {

    // Validate other selections
    this.isUsernameValid = !!this.username;
    this.sameUsername = false;
    if(this.isUsernameValid){
      for(let i=0; i<this.users.length; i++){
        if(this.users[i].username == this.username){
          this.sameUsername = true;
          break;
        } 
      }
    }
    this.rejectedUsername = false;
    if(this.isUsernameValid){
      for(let i=0; i<this.rejected.length; i++){
        if(this.rejected[i].username == this.username){
          this.rejectedUsername = true;
          break;
        } 
      }
    }
    this.isPass1Valid = !!this.password1;
    this.isPass2Valid = !!this.password2;
    this.samePass = true;
    if(this.password1 != this.password2 && this.isPass1Valid && this.isPass2Valid){
      this.samePass = false;
    }
    this.passForm = true;
    if(this.isPass1Valid && this.isPass2Valid){
      this.passForm = this.validatePassword(this.password1);
    }
    this.isFirstnameValid = !!this.firstname;
    this.isLastnameValid = !!this.lastname;
    this.isAdressValid = !!this.adress;
    this.isMailValid = !!this.mail;
    this.usedMail = false;
    if(this.isMailValid){
      for(let i=0; i<this.users.length; i++){
        if(this.users[i].mail == this.mail){
          this.usedMail = true;
          break;
        }
      }
    }
    this.rejectedMail = false;
    if(this.isMailValid){
      for(let i=0; i<this.rejected.length; i++){
        if(this.rejected[i].mail == this.mail){
          this.rejectedMail = true;
          break;
        }
      }
    }
    this.isPhoneValid = !!this.phone;
  
    // Check if all inputs are valid
    if (this.isUsernameValid && !this.sameUsername && this.isPass1Valid && this.isPass2Valid && 
      this.isFirstnameValid && this.isLastnameValid && this.isAdressValid && this.isMailValid && 
      !this.usedMail && this.isPhoneValid && this.samePass && this.passForm && !this.rejectedUsername && !this.rejectedMail) {
        if(this.imageUploaded && !this.wrongSize){
          this.image = this.imagePreview;
        }
        else{
          this.image = 'assets/unknown-person.jpg'
        }
        this.userService.addUser(this.username,this.password1,'patient',this.adress,
        '', this.firstname, this.lastname, 0, this.mail,
        this.phone, '', this.image, false).subscribe(respObj=>{
          if(respObj['message']=='ok'){
            this.message = 'Successful registration'
            alert(this.message)
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

    imagePreview: string;
    imageUploaded = false;
    wrongSize = false;

    onFileSelected(event: any): void {
      const file: File = event.target.files[0];

      if(!file){
        this.imageUploaded = false;
        return;
      }
      this.wrongSize = false;
    
      const reader = new FileReader();
      reader.onload = (e) => {
        this.imagePreview = reader.result as string;
      };
      reader.readAsDataURL(file);

      let img = new Image()
      img.src = window.URL.createObjectURL(event.target.files[0])
      img.onload = () => {
        if(img.height > 300 ||img.height <100 || img.width <100 || img.width >300 ) {
          this.imagePreview = null;
          this.wrongSize = true;

        }
        else{
          this.imageUploaded = true;
        }
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
