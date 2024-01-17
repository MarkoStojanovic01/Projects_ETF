import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';
import { ExaminationsService } from '../services/examinations.service';
import { Specialization } from '../models/specialization';

@Component({
  selector: 'app-update-doctor',
  templateUrl: './update-doctor.component.html',
  styleUrls: ['./update-doctor.component.css']
})
export class UpdateDoctorComponent implements OnInit {

  constructor(private router: Router, private userService: UserService,
    private examinationService: ExaminationsService) { }

  user: User = null;
  users: User[] = [];
  returnAdress: string = '';
  specializations: Specialization[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor' && type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    let doctor = localStorage.getItem("doctor");
    doctor = doctor == null ? "" : doctor; // check if patient wasnt set
    if(doctor == "") this.logout;
    this.userService.getUser(doctor).subscribe((u: User) => {
      this.user = u;
    })
    this.returnAdress = localStorage.getItem("return");
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

  goBack(){
    this.router.navigate([this.returnAdress]);
  }

  message: string;
  firstname: string;
  lastname: string;
  adress: string;
  mail: string;
  phone: string;
  image: string;
  specialization: string;
  licence: number;

  isMailValid = true;

  validateIntegerInput(event: any): void {
    if ((event.which < 48 || event.which > 57) && event.which !== 8) {
      event.preventDefault();
    }
  }
  

  updatePatient(): void {

  if (true) {

    if(this.firstname == '') this.firstname = this.user.firstname;
    if(this.lastname == '') this.lastname = this.user.lastname;
    if(this.adress == '') this.adress = this.user.adress;
    if(this.mail == '') this.mail = this.user.mail;
    if(this.phone == '') this.phone = this.user.phone;
    if(!this.licence) this.licence = this.user.licenceNumber;
    if(this.specialization == '') this.specialization = this.user.specialization;

    this.userService.updateDoctor(this.user.username,this.firstname, 
      this.lastname, this.phone, this.adress,this.licence, this.specialization).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully updated!'
        alert(this.message)
        this.userService.getUser(this.user.username).subscribe((u: User) => {
          this.user = u;
        })
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


  addImage(){
    if(this.imageUploaded && !this.wrongSize){
      this.image = this.imagePreview;
      this.userService.addImage(this.user.username,this.image).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully added image!'
          alert(this.message)
          this.userService.getUser(this.user.username).subscribe((u: User) => {
            this.user = u;
          })
        }
        else{
          this.message = 'Error'
          alert(this.message)  
        }
      });
    }
    else{
      this.message = 'Image is not uploaded or wrong size!'
      alert(this.message) 
    }
  }

  removeImage(){
    this.image = this.image = 'assets/unknown-person.jpg';
      this.userService.addImage(this.user.username,this.image).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully removed image!'
          alert(this.message)
          this.userService.getUser(this.user.username).subscribe((u: User) => {
            this.user = u;
          })
        }
        else{
          this.message = 'Error!'
          alert(this.message)  
        }
      });
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

}
