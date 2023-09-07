import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';
import { LoginComponent } from './login/login.component';
import { PatientComponent } from './patient/patient.component';
import { HospitalComponent } from './hospital/hospital.component';
import { PatientDoctorsComponent } from './patient-doctors/patient-doctors.component';
import { PatientDocProfileComponent } from './patient-doc-profile/patient-doc-profile.component';
import { PatientExaminationsComponent } from './patient-examinations/patient-examinations.component';
import { DoctorComponent } from './doctor/doctor.component';
import { DoctorExaminationsComponent } from './doctor-examinations/doctor-examinations.component';
import { DoctorPatientComponent } from './doctor-patient/doctor-patient.component';
import { DoctorOtherComponent } from './doctor-other/doctor-other.component';
import { ManagerComponent } from './manager/manager.component';
import { ManagerUserComponent } from './manager-user/manager-user.component';
import { ManagerDoctorComponent } from './manager-doctor/manager-doctor.component';
import { UpdatePatientComponent } from './update-patient/update-patient.component';

const routes: Routes = [
  {path: 'login', component: LoginComponent},
  {path: 'patient', component: PatientComponent},
  {path: '', component: HospitalComponent},
  {path: 'patient/doctors', component: PatientDoctorsComponent},
  {path: 'patient/doc/:username', component: PatientDocProfileComponent},
  {path: 'patient/examinations', component: PatientExaminationsComponent},
  {path: 'doctor', component: DoctorComponent},
  {path: 'doctor/examinations', component: DoctorExaminationsComponent},
  {path: 'doctor/patient/:username', component: DoctorPatientComponent},
  {path: 'doctor/other', component: DoctorOtherComponent},
  {path: 'manager', component: ManagerComponent},
  {path: 'manager/user/:username', component: ManagerUserComponent},
  {path: 'manager/doctor', component: ManagerDoctorComponent},
  {path: 'updatePatient', component: UpdatePatientComponent}
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
