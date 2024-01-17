import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { UserService } from '../services/user.service';
import { User } from '../models/user';
import { AllExamination } from '../models/all_examination';
import { DoctorExamination } from '../models/doctor_examination';
import { ExaminationsService } from '../services/examinations.service';

@Component({
  selector: 'app-doctor',
  templateUrl: './doctor.component.html',
  styleUrls: ['./doctor.component.css']
})
export class DoctorComponent implements OnInit {

  constructor(private router: Router, private userService: UserService,
    private examinationService: ExaminationsService, private route: ActivatedRoute) { }

  user: User = null;
  allExaminations: AllExamination[] = [];
  doctorExaminations: DoctorExamination[] = [];
  message: string;

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getUser(username).subscribe((u: User) => {
      this.user = u;
      this.examinationService.getSpecExaminations(u.specialization).subscribe((data: AllExamination[])=>{
        this.allExaminations = data;
      })
    })
    this.examinationService.getDocExaminations(username).subscribe((data: DoctorExamination[])=>{
      this.doctorExaminations = data;
    })
    
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  addExamination(examination,duration,price) {
    this.examinationService.addDocExamination(examination,this.user.specialization,duration,price,this.user.username).subscribe(respObj=>{
      if(respObj['message']=='ok'){
        this.message = 'Successfully added examination'
        this.examinationService.getDocExaminations(this.user.username).subscribe((data: DoctorExamination[])=>{
          this.doctorExaminations = data;
        })
        alert(this.message)
      }
      else{
        this.message = 'Error'
        alert(this.message)
        
      }
    });
  }

  alreadyHave(examination) : boolean{
    for(let i=0; i<this.doctorExaminations.length; i++){
      if(this.doctorExaminations[i].examination == examination) return false;
    }
    return true;
  }

  updateUser(){
    localStorage.setItem('doctor',this.user.username);
    this.route.url.subscribe(urlSegments => {
      const path = urlSegments.map(segment => segment.path).join('/');
      localStorage.setItem('return',path);
      this.router.navigate(['updateDoctor'])
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
