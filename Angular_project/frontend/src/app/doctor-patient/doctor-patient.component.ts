import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { ExaminationsService } from '../services/examinations.service';
import { UserService } from '../services/user.service';
import { User } from '../models/user';
import { Report } from '../models/report';
import { Appointment } from '../models/appointment';


@Component({
  selector: 'app-doctor-patient',
  templateUrl: './doctor-patient.component.html',
  styleUrls: ['./doctor-patient.component.css']
})
export class DoctorPatientComponent implements OnInit {

  constructor(private route: ActivatedRoute, private router: Router,
    private userService: UserService, private examinationService: ExaminationsService) { }


  doctor: User;
  patient: User;
  reports: Report[] = [];
  appointments: Appointment[] = [];

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'doctor') this.router.navigate(['']);
    let patientUsername = this.route.snapshot.paramMap.get('username');
    let doctorUsername = localStorage.getItem("user");
    doctorUsername = doctorUsername == null ? "" : doctorUsername; // check if username wasnt set
    if(doctorUsername == "") this.logout;
    this.userService.getUser(patientUsername).subscribe((u: User) => {
      if(u == null) this.router.navigate(['']);
      this.patient = u;
    })
    this.userService.getUser(doctorUsername).subscribe((u: User) => {
      this.doctor = u;
    })
    this.examinationService.getPatientDocReports(patientUsername,doctorUsername).subscribe((data: Report[]) => {
      this.reports = data;
      this.examinationService.getPatientDocAppointments(patientUsername,doctorUsername).subscribe((data: Appointment[]) => {
        this.appointments = data;
        const currentDate = new Date();
        this.appointments.forEach((element,index)=>{
          const appDate = new Date(element.date)
          if(appDate > currentDate) delete this.appointments[index];
          for(let i=0;i<this.reports.length;i++){
            if(element.date == this.reports[i].date){
              delete this.appointments[index];
              break;
            }
          }
         });
        this.appointments = this.appointments.sort((a, b) =>  {
          if(a.date > b.date) return 1;
          else return -1
        });
      })
    })
    
  }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }


  appointmentDate: Date;
  message: string;
  diagnosis: string;
  reason: string;
  therapy: string;
  newAppointmentDate: string;


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

    this.examinationService.addReport(this.doctor.specialization,this.doctor.username,
      this.doctor.firstname,this.doctor.lastname,this.patient.username,
      this.appointmentDate,this.diagnosis,this.reason,this.therapy,newAppointmentDateTime).subscribe(respObj=>{
        if(respObj['message']=='ok'){
          this.message = 'Successfully added report'
          // load again
          this.examinationService.getPatientDocReports(this.patient.username,this.doctor.username).subscribe((data: Report[]) => {
            this.reports = data;
            this.examinationService.getPatientDocAppointments(this.patient.username,this.doctor.username).subscribe((data: Appointment[]) => {
              this.appointments = data;
              const currentDate = new Date();
              this.appointments.forEach((element,index)=>{
                const appDate = new Date(element.date)
                if(appDate > currentDate) delete this.appointments[index];
                for(let i=0;i<this.reports.length;i++){
                  if(element.date == this.reports[i].date){
                    delete this.appointments[index];
                    break;
                  }
                }
               });
              this.appointments = this.appointments.sort((a, b) =>  {
                if(a.date > b.date) return 1;
                else return -1
              });
            })
          })

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

}
