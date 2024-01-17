import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { UserService } from '../services/user.service';
import { User } from '../models/user';
import { Specialization } from '../models/specialization';
import { ExaminationsService } from '../services/examinations.service';
import { HttpClient } from '@angular/common/http';

@Component({
  selector: 'app-manager-doctor',
  templateUrl: './manager-doctor.component.html',
  styleUrls: ['./manager-doctor.component.css']
})
export class ManagerDoctorComponent implements OnInit {

  constructor(private router: Router, private userService: UserService,
    private examinationService: ExaminationsService, private http: HttpClient) { }

  users: User[] = [];
  specializations: Specialization[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getAllUsers().subscribe((data:User[])=>{
      this.users = data;
    })
    this.examinationService.getAllSpecializations().subscribe((data:Specialization[])=>{
      this.specializations = data;
    })
    
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
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
  licence: number;
  specialization: string;
  branch: string;
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
  isSpecializationValid = true;
  isBranchValid = true;
  isLicenceValid = true;

  sameUsername = false;
  samePass = true;
  usedMail = false;
  passForm = true;

  async addDoctor(): Promise<void> {

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
    this.isPhoneValid = !!this.phone;
    this.isSpecializationValid = !!this.specialization;
    this.isBranchValid = !!this.branch;
    this.isLicenceValid = !!this.licence;
  
    // Check if all inputs are valid
    if (this.isUsernameValid && !this.sameUsername && this.isPass1Valid && this.isPass2Valid && 
      this.isFirstnameValid && this.isLastnameValid && this.isAdressValid && this.isMailValid && 
      !this.usedMail && this.isPhoneValid && this.isSpecializationValid && this.isBranchValid && 
      this.samePass && this.passForm && this.isLicenceValid) {
        if(this.imageUploaded && !this.wrongSize){
          this.image = this.imagePreview;
        }
        else{
          this.image = 'assets/unknown-person.jpg'
        }
        this.userService.addUser(this.username,this.password1,'doctor',this.adress,
        this.branch, this.firstname, this.lastname, this.licence, this.mail,
        this.phone, this.specialization, this.image, true).subscribe(respObj=>{
          if(respObj['message']=='ok'){
            this.message = 'Successfully added doctor'
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
