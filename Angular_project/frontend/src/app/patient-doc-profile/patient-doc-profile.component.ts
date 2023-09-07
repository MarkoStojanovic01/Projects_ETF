import { Component, OnInit } from '@angular/core';
import { ActivatedRoute, Router } from '@angular/router';
import { User } from '../models/user';
import { UserService } from '../services/user.service';
import { ExaminationsService } from '../services/examinations.service';
import { DoctorExamination } from '../models/doctor_examination';
import { Appointment } from '../models/appointment';

@Component({
  selector: 'app-patient-doc-profile',
  templateUrl: './patient-doc-profile.component.html',
  styleUrls: ['./patient-doc-profile.component.css']
})
export class PatientDocProfileComponent implements OnInit {

  patientUsername;
  doctor: User;
  examinations: DoctorExamination[] = [];
  appointments: Appointment[] = [];

  constructor(private route: ActivatedRoute, private router: Router,
    private userService: UserService, private examinationService: ExaminationsService) { }

  logout() {
    localStorage.clear()
    this.router.navigate([''])
  }

  ngOnInit(): void {
    let type = localStorage.getItem("type");
    if(type != 'patient') this.router.navigate(['']);
    let doctorUsername = this.route.snapshot.paramMap.get('username');
    let username = localStorage.getItem("user");
    username = username == null ? "" : username; // check if username wasnt set
    if(username == "") this.logout;
    this.patientUsername = username;
    this.userService.getUser(doctorUsername).subscribe((u: User) => {
      if(u == null) this.router.navigate(['']);
      this.doctor = u;
    })
    this.examinationService.getDocExaminations(doctorUsername).subscribe((data: DoctorExamination[]) => {
      this.examinations = data;
    })
    this.examinationService.getDocAppointments(doctorUsername).subscribe((data: Appointment[]) => {
      this.appointments = data;
    })

  }

  selectedExamination: string;
  appointmentDate: string;
  appointmentTime: string;
  message: string;

  // Initialize boolean flags for input validation
  isDateValid = true;
  isTimeValid = true;
  isExaminationValid = true;

  scheduleAppointment(): void {
    // Validate date format (dd.mm.yyyy)
  const dateRegex = /^\d{2}\.\d{2}\.\d{4}\.$/;
  this.isDateValid = dateRegex.test(this.appointmentDate);

  // Validate time format (hh:mm)
  const timeRegex = /^\d{2}:\d{2}$/;
  this.isTimeValid = timeRegex.test(this.appointmentTime);

  if (this.isTimeValid) {
    const [hours, minutes] = this.appointmentTime.split(':');
    const isValidHours = Number(hours) >= 0 && Number(hours) < 24;
    const isValidMinutes = Number(minutes) >= 0 && Number(minutes) < 60;
    this.isTimeValid = isValidHours && isValidMinutes;
  }

  // Validate examination selection
  this.isExaminationValid = !!this.selectedExamination;

  // Check if all inputs are valid
  if (this.isDateValid && this.isTimeValid && this.isExaminationValid) {

    const appointmentDateTime = new Date(this.appointmentDate);
    const appointmentTimeParts = this.appointmentTime.split(':');
    appointmentDateTime.setHours(Number(appointmentTimeParts[0]));
    appointmentDateTime.setMinutes(Number(appointmentTimeParts[1]));
    const appointmentDuration = this.examinations.find(e => e.examination === this.selectedExamination)?.duration || 0;

    // Check doctor's availability
    const isAvailable = this.checkDoctorAvailability(appointmentDateTime, appointmentDuration);
    
    if (isAvailable) {
      let price = this.examinations.find(e => e.examination === this.selectedExamination)?.price || '';
      this.examinationService.addDocAppointment(this.selectedExamination,this.doctor.specialization
        ,appointmentDuration,price,this.doctor.username,this.doctor.firstname,
        this.doctor.lastname,this.patientUsername,appointmentDateTime,
        this.doctor.branch).subscribe(respObj=>{
          if(respObj['message']=='ok'){
            this.message = 'Successfully scheduled appointment'
            this.examinationService.getDocAppointments(this.doctor.username).subscribe((data: Appointment[]) => {
              this.appointments = data;
            })
            alert(this.message)
          }
          else{
            this.message = 'Error'
            alert(this.message)
            
          }
        });
    } else {
      // Handle unavailability
      alert('Doctor is not available at the chosen time.');
    }
  }
  else{
    alert('Please fix the form errors before submitting.');
  }
    
  }

  checkDoctorAvailability(appointmentDateTime: Date, duration: number): boolean {
    // Iterate through existing appointments and check for overlap
  for (const appointment of this.appointments) {
    const existingAppointmentStart = new Date(appointment.date);
    const existingAppointmentEnd = new Date(existingAppointmentStart);
    existingAppointmentEnd.setMinutes(existingAppointmentEnd.getMinutes() + appointment.duration);

    const newAppointmentEnd = new Date(appointmentDateTime);
    newAppointmentEnd.setMinutes(newAppointmentEnd.getMinutes() + duration);

    // Check for overlap by comparing time ranges
    if (
      (appointmentDateTime >= existingAppointmentStart && appointmentDateTime < existingAppointmentEnd) ||
      (newAppointmentEnd > existingAppointmentStart && newAppointmentEnd <= existingAppointmentEnd)
    ) {
      return false; // Overlapping appointment found
    }
  }

  return true;
 } 
}
