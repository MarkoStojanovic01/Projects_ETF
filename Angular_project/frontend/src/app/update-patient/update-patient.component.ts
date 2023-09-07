import { Component, OnInit } from '@angular/core';
import { User } from '../models/user';
import { Router } from '@angular/router';
import { UserService } from '../services/user.service';

@Component({
  selector: 'app-update-patient',
  templateUrl: './update-patient.component.html',
  styleUrls: ['./update-patient.component.css']
})
export class UpdatePatientComponent implements OnInit {

  constructor(private router: Router, private userService: UserService) { }

  user: User = null;
  users: User[] = [];
  returnAdress: string = '';

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient' && type != 'manager') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    let patient = localStorage.getItem("patient");
    patient = patient == null ? "" : patient; // check if patient wasnt set
    if(patient == "") this.logout;
    this.userService.getUser(patient).subscribe((u: User) => {
      this.user = u;
    })
    this.returnAdress = localStorage.getItem("return");
    this.userService.getAllUsers().subscribe((data:User[])=>{
      this.users = data;
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

  isMailValid = true;
  

  updatePatient(): void {

    this.isMailValid = true;
    if(this.mail != ''){
      for(let i=0; i<this.users.length; i++){
        if(this.users[i].mail == this.mail){
          this.isMailValid = false;
          break;
        }
      }
    }

  if (this.isMailValid) {

    if(this.firstname == '') this.firstname = this.user.firstname;
    if(this.lastname == '') this.lastname = this.user.lastname;
    if(this.adress == '') this.adress = this.user.adress;
    if(this.mail == '') this.mail = this.user.mail;
    if(this.phone == '') this.phone = this.user.phone;

    this.userService.updatePatient(this.user.username,this.firstname, 
      this.lastname, this.mail, this.phone, this.adress,).subscribe(respObj=>{
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
