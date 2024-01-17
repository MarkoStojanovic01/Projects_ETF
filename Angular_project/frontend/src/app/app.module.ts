import { NgModule } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { FormsModule } from '@angular/forms';
import { HttpClientModule } from '@angular/common/http';
import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { LoginComponent } from './login/login.component';
import { PatientComponent } from './patient/patient.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { HospitalComponent } from './hospital/hospital.component';
import { PatientDoctorsComponent } from './patient-doctors/patient-doctors.component';
import { PatientDocProfileComponent } from './patient-doc-profile/patient-doc-profile.component';
import { PatientExaminationsComponent } from './patient-examinations/patient-examinations.component';
import { DatePipe } from '@angular/common';
import { DoctorComponent } from './doctor/doctor.component';
import { DoctorExaminationsComponent } from './doctor-examinations/doctor-examinations.component';
import { DoctorPatientComponent } from './doctor-patient/doctor-patient.component';
import { DoctorOtherComponent } from './doctor-other/doctor-other.component';
import { ManagerComponent } from './manager/manager.component';
import { ManagerUserComponent } from './manager-user/manager-user.component';
import { ManagerDoctorComponent } from './manager-doctor/manager-doctor.component';
import { UpdatePatientComponent } from './update-patient/update-patient.component';
import { UpdateDoctorComponent } from './update-doctor/update-doctor.component';
import { PasswordComponent } from './password/password.component';
import { LoginManagerComponent } from './login-manager/login-manager.component';
import { RegisterComponent } from './register/register.component';
import { RegistrationsComponent } from './registrations/registrations.component';
import { ManagerExaminationsComponent } from './manager-examinations/manager-examinations.component';

@NgModule({
  declarations: [
    AppComponent,
    LoginComponent,
    PatientComponent,
    HospitalComponent,
    PatientDoctorsComponent,
    PatientDocProfileComponent,
    PatientExaminationsComponent,
    DoctorComponent,
    DoctorExaminationsComponent,
    DoctorPatientComponent,
    DoctorOtherComponent,
    ManagerComponent,
    ManagerUserComponent,
    ManagerDoctorComponent,
    UpdatePatientComponent,
    UpdateDoctorComponent,
    PasswordComponent,
    LoginManagerComponent,
    RegisterComponent,
    RegistrationsComponent,
    ManagerExaminationsComponent
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    FormsModule,
    HttpClientModule,
    BrowserAnimationsModule
  ],
  providers: [DatePipe],
  bootstrap: [AppComponent]
})
export class AppModule { }
