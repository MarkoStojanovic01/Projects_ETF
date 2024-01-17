import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';
import { Appointment } from '../models/appointment';
import { ExaminationsService } from '../services/examinations.service';
import { Report } from '../models/report';
import { User } from '../models/user';
import { UserService } from '../services/user.service';

@Component({
  selector: 'app-doctor-examinations',
  templateUrl: './doctor-examinations.component.html',
  styleUrls: ['./doctor-examinations.component.css']
})
export class DoctorExaminationsComponent implements OnInit {

  doctor: User;
  appointments: Appointment[] = [];
  oldAppointments: Appointment[] = [];
  reports: Report[] = [];

  constructor(private router: Router,private examinationService: ExaminationsService,
    private userService: UserService) { }

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor') this.router.navigate(['']);
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.userService.getUser(username).subscribe((u: User) =>{
      this.doctor = u;
    })
    this.examinationService.getDocReports(username).subscribe((data: Report[]) => {
      this.reports = data;

      this.examinationService.getDocAppointments(username).subscribe((data: Appointment[]) => {
        this.oldAppointments = data;
        const currentDate = new Date();
       this.oldAppointments.forEach((element,index)=>{
        const appDate = new Date(element.date)
        if(appDate > currentDate) delete this.oldAppointments[index];
        for(let i=0;i<this.reports.length;i++){
          if(element.date == this.reports[i].date){
            delete this.oldAppointments[index];
            break;
          }
        }
        });
      this.oldAppointments = this.oldAppointments.sort((a, b) =>  {
        if(a.date > b.date) return 1;
        else return -1
      });
      })

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
      
    })
    
    
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  removeAppointment(doc, date){
    this.examinationService.removeDocAppointment(doc,date).subscribe(resp=>{
      alert(resp['message'])
      this.examinationService.getDocAppointments(this.doctor.username).subscribe((data: Appointment[]) => {
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

  appointmentDate: Date;
  message: string;
  diagnosis: string;
  reason: string;
  therapy: string;
  newAppointmentDate: string;
  patient: string;


  // Initialize boolean flags for input validation
  isDateValid = true;
  isExaminationValid = true;
  isDiagnosisValid = true;
  isReasonValid = true;
  isTherapyValid = true;

  makeReport(): void {
    // Validate date format (dd.mm.yyyy)
  const dateRegex = /^\d{2}\.\d{2}\.\d{4}\.$/;
  this.isDateValid = dateRegex.test(this.newAppointmentDate);


  // Validate other selections
  this.isExaminationValid = !!this.appointmentDate;
  this.isDiagnosisValid = !!this.diagnosis;
  this.isReasonValid = !!this.reason;
  this.isTherapyValid = !!this.therapy;

  // Check if all inputs are valid
  if (this.isDateValid  && this.isExaminationValid && this.isDiagnosisValid 
    && this.isReasonValid && this.isTherapyValid) {

    const newAppointmentDateTime = new Date(this.newAppointmentDate);

    this.patient = this.getPatient(this.appointmentDate);

    this.examinationService.addReport(this.doctor.specialization,this.doctor.username,
      this.doctor.firstname,this.doctor.lastname,this.patient,
      this.appointmentDate,this.diagnosis,this.reason,this.therapy,newAppointmentDateTime).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully added report'
          alert(this.message)
          for(let i=0; i<this.oldAppointments.length; i++){
            if(this.oldAppointments[i].date == this.appointmentDate){
              delete this.oldAppointments[i]
            }
          }
          
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

  getPatient(date: Date): string{
    for(let i=0; i<this.oldAppointments.length; i++){
      if(this.oldAppointments[i].date == date) return this.oldAppointments[i].patient;
    }
    return ''
  }

}
