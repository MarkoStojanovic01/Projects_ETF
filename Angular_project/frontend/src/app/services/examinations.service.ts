import { HttpClient } from '@angular/common/http';
import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class ExaminationsService {

  constructor(private http: HttpClient) { }

  uri = "http://localhost:4000/examinations";

  getDocExaminations(username: string) {
    let data = {
      username: username
    }
    return this.http.post(`${this.uri}/getDocExaminations`, data)
  }

  getDocAppointments(doc: string) {
    let data = {
      doc: doc
    }
    return this.http.post(`${this.uri}/getDocAppointments`, data)
  }

  getPatientAppointments(patient: string) {
    let data = {
      patient: patient
    }
    return this.http.post(`${this.uri}/getPatientAppointments`, data)
  }

  getPatientDocAppointments(patient: string, doc: string) {
    let data = {
      patient: patient,
      doc: doc
    }
    return this.http.post(`${this.uri}/getPatientDocAppointments`, data)
  }

  addDocAppointment(examination,specialization,duration,price,doc_username,
    doc_firstname,doc_lastname,patient,date,branch) {
    let data = {
      examination: examination,
      specialization: specialization,
      duration: duration,
      price: price,
      doc_username: doc_username,
      doc_firstname: doc_firstname,
      doc_lastname: doc_lastname,
      patient: patient,
      date: date,
      branch: branch
    }
    return this.http.post(`${this.uri}/addDocAppointment`, data)
  }

  getPatientReports(patient: string) {
    let data = {
      patient: patient
    }
    return this.http.post(`${this.uri}/getPatientReports`, data)
  }

  getPatientDocReports(patient: string, doc: string) {
    let data = {
      patient: patient,
      doc: doc
    }
    return this.http.post(`${this.uri}/getPatientDocReports`, data)
  }

  getDocReports(doc: string) {
    let data = {
      doc: doc
    }
    return this.http.post(`${this.uri}/getDocReports`, data)
  }

  getDocReport(doc: string, date: Date) {
    let data = {
      doc: doc,
      date: date
    }
    return this.http.post(`${this.uri}/getDocReport`, data)
  }

  removeDocAppointment(doc: string, date: Date){
    let data = {
      doc: doc,
      date: date
    }
    return this.http.post(`${this.uri}/removeDocAppointment`, data)
  }

  getSpecExaminations(specialization: string) {
    let data = {
      specialization: specialization
    }
    return this.http.post(`${this.uri}/getSpecExaminations`, data)
  }

  addDocExamination(examination,specialization,duration,price,doc) {
    let data = {
      examination: examination,
      specialization: specialization,
      duration: duration,
      price: price,
      doc: doc
    }
    return this.http.post(`${this.uri}/addDocExamination`, data)
  }

  requestExamination(examination,specialization,duration,price) {
    let data = {
      examination: examination,
      specialization: specialization,
      duration: duration,
      price: price
    }
    return this.http.post(`${this.uri}/requestExamination`, data)
  }

  addReport(specialization,doc_username,doc_firstname,
    doc_lastname,patient,date,diagnosis,reason,therapy,nextDate) {
    let data = {
      specialization: specialization,
      doc_username: doc_username,
      doc_firstname: doc_firstname,
      doc_lastname: doc_lastname,
      patient: patient,
      date: date,
      diagnosis: diagnosis,
      reason: reason,
      therapy: therapy,
      nextDate: nextDate
    }
    return this.http.post(`${this.uri}/addReport`, data)
  }

  getAllSpecializations(){
    return this.http.get(`${this.uri}/getAllSpecializations`)
  }

  getNewExaminations(){
    return this.http.get(`${this.uri}/getNewExaminations`)
  }

  acceptExamination(examination,specialization) {
    let data = {
      examination: examination,
      specialization: specialization
    }
    return this.http.post(`${this.uri}/acceptExamination`, data)
  }

  removeExamination(examination,specialization) {
    let data = {
      examination: examination,
      specialization: specialization,
      added: 'false'
    }
    return this.http.post(`${this.uri}/removeExamination`, data)
  }

  deleteExamination(examination,specialization) {
    let data = {
      examination: examination,
      specialization: specialization,
      added: 'true'
    }
    return this.http.post(`${this.uri}/removeExamination`, data)
  }

  addSpecialization(specialization) {
    let data = {
      specialization: specialization
    }
    return this.http.post(`${this.uri}/addSpecialization`, data)
  }

  updateExamination(oldExamination,examination, specialization, duration, price){
    let data = {
      oldExamination: oldExamination,
      examination: examination,
      specialization: specialization,
      duration: duration,
      price: price
    }
    return this.http.post(`${this.uri}/updateExamination`, data)
  }

  addExamination(examination,specialization,duration,price) {
    let data = {
      examination: examination,
      specialization: specialization,
      duration: duration,
      price: price
    }
    return this.http.post(`${this.uri}/addExamination`, data)
  }
}
