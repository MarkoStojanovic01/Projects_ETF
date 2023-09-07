import { Component, OnInit } from '@angular/core';
import { User } from '../models/user';
import { UserService } from '../services/user.service';
import { Router } from '@angular/router';
import { ExaminationsService } from '../services/examinations.service';

@Component({
  selector: 'app-doctor-other',
  templateUrl: './doctor-other.component.html',
  styleUrls: ['./doctor-other.component.css']
})
export class DoctorOtherComponent implements OnInit {

  constructor(private userService: UserService, private router: Router,
    private examinationService: ExaminationsService) { }

  user: User;

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor') this.router.navigate(['']);
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

  message: string;
  examination: string;
  duration: number;
  price: string;

  // Initialize boolean flags for input validation
  isExaminationValid = true;
  isPriceValid = true;
  isDurationValid = true;

  requestExamination(): void {

  // Validate other selections
  this.isExaminationValid = !!this.examination;
  this.isPriceValid = !!this.price;
  this.isDurationValid = !!this.duration;

  // Check if all inputs are valid
  if (this.isExaminationValid && this.isPriceValid && (this.duration > 0) && this.isDurationValid) {

      this.examinationService.requestExamination(this.examination,this.user.specialization,this.duration,this.price).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully added examination'
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

  validateIntegerInput(event: any): void {
    if ((event.which < 48 || event.which > 57) && event.which !== 8) {
      event.preventDefault();
    }
  }

}
