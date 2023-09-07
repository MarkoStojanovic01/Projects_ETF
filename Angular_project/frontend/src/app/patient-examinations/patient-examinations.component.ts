import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { UserService } from '../services/user.service';
import { ExaminationsService } from '../services/examinations.service';
import { Appointment } from '../models/appointment';
import { Report} from '../models/report'
import { DatePipe, formatDate } from '@angular/common';

@Component({
  selector: 'app-patient-examinations',
  templateUrl: './patient-examinations.component.html',
  styleUrls: ['./patient-examinations.component.css']
})
export class PatientExaminationsComponent implements OnInit {

  constructor(private router: Router,private userService: UserService, 
    private examinationService: ExaminationsService,private datePipe: DatePipe) { }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  appointments: Appointment[] = [];
  reports: Report[] = [];
  patient: string;
  today: Date;

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.patient = username;
    this.examinationService.getPatientReports(username).subscribe((data: Report[]) => {
      this.reports = data;
    })
    this.examinationService.getPatientAppointments(username).subscribe((data: Appointment[]) => {
      const currentDate = new Date();
      this.today = currentDate;
      this.appointments = data;
      this.appointments.forEach((element,index)=>{
        const appDate = new Date(element.date)
        if(appDate < currentDate) delete this.appointments[index];
     });
      this.appointments = this.appointments.sort((a, b) =>  {
        if(a.date > b.date) return 1;
        else return -1
      });
      })
  }

  removeAppointment(doc, date){
    this.examinationService.removeDocAppointment(doc,date).subscribe(resp=>{
      alert(resp['message'])
      this.examinationService.getPatientAppointments(this.patient).subscribe((data: Appointment[]) => {
        const currentDate = new Date();
        this.today = currentDate;
        this.appointments = data;
        this.appointments.forEach((element,index)=>{
          const appDate = new Date(element.date)
          if(appDate < currentDate) delete this.appointments[index];
       });
        this.appointments = this.appointments.sort((a, b) =>  {
          if(a.date > b.date) return 1;
          else return -1
        });
        })
    })
  }




}
