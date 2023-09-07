import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Appointment } from '../models/appointment';
import { ExaminationsService } from '../services/examinations.service';

@Component({
  selector: 'app-doctor-examinations',
  templateUrl: './doctor-examinations.component.html',
  styleUrls: ['./doctor-examinations.component.css']
})
export class DoctorExaminationsComponent implements OnInit {

  doctor: string;
  appointments: Appointment[] = [];

  constructor(private router: Router,private examinationService: ExaminationsService) { }

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.doctor = username;
    this.examinationService.getDocAppointments(username).subscribe((data: Appointment[]) => {
      this.appointments = data;
      const currentDate = new Date();
      this.appointments.forEach((element,index)=>{
        const appDate = new Date(element.date)
        if(appDate < currentDate) delete this.appointments[index];
       });
      this.appointments = this.appointments.sort((a, b) =>  {
        if(a.date > b.date) return 1;
        else return -1
      });
      this.appointments.forEach((element,index)=>{
        if(index > 2) delete this.appointments[index];
     });
    })
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  removeAppointment(doc, date){
    this.examinationService.removeDocAppointment(doc,date).subscribe(resp=>{
      alert(resp['message'])
      this.examinationService.getDocAppointments(this.doctor).subscribe((data: Appointment[]) => {
        this.appointments = data;
        const currentDate = new Date();
        this.appointments.forEach((element,index)=>{
          const appDate = new Date(element.date)
          if(appDate < currentDate) delete this.appointments[index];
        });
        this.appointments = this.appointments.sort((a, b) =>  {
          if(a.date > b.date) return 1;
          else return -1
        });
        this.appointments.forEach((element,index)=>{
          if(index > 2) delete this.appointments[index];
        });
    })
  })
  }

}
